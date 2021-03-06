﻿using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace OML
{
    public class Handler
    {
        public Process proc;
        public int IsWorking = 0;
        public int Abort = 0;
        private MainWindow mw;
        public ConcurrentQueue<Command> commandQueue = new ConcurrentQueue<Command>();

        public Dictionary<int, bool> hasCustomItem = new Dictionary<int, bool>();

        public Handler(MainWindow mw)
        {
            this.mw = mw;
        }

        public void Handle()
        {
            //init server and streams
            using (var server = new NamedPipeServerStream("omlpipe", PipeDirection.InOut, NamedPipeServerStream.MaxAllowedServerInstances, PipeTransmissionMode.Message, PipeOptions.WriteThrough, 32768, 32768))
            {
                var ServerIn = new BinaryReader(server);
                var ServerOut = new BinaryWriter(server);
                var formatter = new BinaryFormatter();

                mw.WriteLine(Level.Info, "Loading plugins...");
                //load plugins
                var plugins = Loader.GetPlugins();
                List<Item> items = Loader.LoadItems(plugins, mw.path);
                foreach (Item i in items)
                    hasCustomItem.Add(i.id, false);
                foreach (OMLPlugin p in plugins)
                {
                    mw.WriteLine(Level.Info, "    Loading {0}-v{1} by {2} ...", p.PluginName, p.PluginVersion, p.PluginAuthor);
                    p.PluginInit();
                }
                mw.WriteLine(Level.Info, "Plugin load completed.");

                //wait for connection on event pipe
                mw.WriteLine(Level.Info, "Waiting for connection to event pipe...");
                server.WaitForConnection();

                // connect to API pipe
                mw.WriteLine(Level.Info, "Connecting to call pipe server...");
                NamedPipeClientStream client = new NamedPipeClientStream(".", "omlCallPipe", PipeDirection.InOut, PipeOptions.None);
                client.Connect();
                client.ReadMode = PipeTransmissionMode.Message;

                var ClientIn = new BinaryReader(client);
                var ClientOut = new BinaryWriter(client);

                _OML.Connection = new API_ConnectionInfo(client, ClientIn, ClientOut);

                mw.WriteLine(Level.Info, "Connected.");

                //create items
               // foreach (Item i in items)
               //     API.AddCustomItem(i.id);

                // Peek named pipe arguments
                byte[] eventID = new byte[1];
                uint bytesRead = 0;
                uint bytesAvail = 0;
                uint bytesLeft = 0;

                Interlocked.Exchange(ref IsWorking, 1);

                // while ((!proc.HasExited) && (Abort == 0))
                while ((Abort == 0))
                {
                    try
                    {
                        if (NativeMethods.PeekNamedPipe(server.SafePipeHandle, eventID, 1, ref bytesRead, ref bytesAvail, ref bytesLeft) && (bytesRead > 0))
                        {
                            switch (eventID[0])
                            {
                                case _OML.PLAYER_EVENT_TAKEPILL:
                                    if (bytesLeft + 1 == TakePillEvent_Notification.size())
                                    {
                                        // Receive event
                                        TakePillEvent_Notification notification = RawDeserialize<TakePillEvent_Notification>(ServerIn.ReadBytes(TakePillEvent_Notification.size()), 0);

                                        Player player = new Player(notification.playerHandle);
                                        bool handled = false;

                                        foreach (OMLPlugin p in plugins)
                                        {
                                            p.OnPlayerPillUse(player, notification.pillID, ref handled);
                                            foreach (Item i in p.CustomItemList)
                                                if (hasCustomItem[i.id]) 
                                                    i.OnPlayerPillUse(player, notification.pillID, ref handled);
                                        }

                                        new API_EndCall(_OML.Connection).Call();

                                        server.Flush();

                                        // Send response
                                        TakePillEvent_Response response;
                                        response.eventID = _OML.PLAYER_EVENT_TAKEPILL;
                                        response.handled = handled;

                                        ServerOut.Write(RawSerialize(response));
                                    }
                                    else
                                        mw.WriteLine(Level.Warning, "PLAYER_EVENT_TAKEPILL: expected " + TakePillEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    break;

                                case _OML.PLAYER_EVENT_ADDCOLLECTIBLE:
                                    if (bytesLeft + 1 == AddCollectibleEvent_Notification.size())
                                    {
                                        // Receive event

                                        AddCollectibleEvent_Notification notification = RawDeserialize<AddCollectibleEvent_Notification>(ServerIn.ReadBytes(AddCollectibleEvent_Notification.size()), 0);

                                        Player player = new Player(notification.playerHandle);

                                        if (notification.itemID < 0) //it's ours
                                            hasCustomItem[notification.itemID] = true;

                                        foreach (OMLPlugin p in plugins)
                                        {
                                            p.OnPlayerAddCollectible(player, notification.itemID, notification.charges, notification.a4);
                                            foreach (Item i in p.CustomItemList)
                                            {
                                                mw.WriteLine(Level.Info, "OnPlayerAddCollectible: i.id = " + i.id + " (" + notification.itemID + " )");
                                                if (i.id == notification.itemID)
                                                    i.OnPickup(player);
                                                //else
                                                //if (hasCustomItem[i.id])
                                                //    i.OnPlayerAddCollectible(player, notification.a2, notification.itemID, notification.a4);
                                            }
                                        }

                                        new API_EndCall(_OML.Connection).Call();

                                        server.Flush();

                                        // Send response
                                        AddCollectibleEvent_Response response;
                                        response.eventID = _OML.PLAYER_EVENT_ADDCOLLECTIBLE;

                                        ServerOut.Write(RawSerialize(response));

                                    }
                                    else
                                        mw.WriteLine(Level.Warning, "PLAYER_EVENT_ADDCOLLECTIBLE: expected " + AddCollectibleEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    break;

                                case _OML.GAME_EVENT_SPAWNENTITY:
                                    if (bytesLeft + 1 == SpawnEntityEvent_Notification.size())
                                    {
                                        SpawnEntityEvent_Notification notification = RawDeserialize<SpawnEntityEvent_Notification>(ServerIn.ReadBytes(SpawnEntityEvent_Notification.size()), 0);

                                        // Receive event


                                        Entity parent = new Entity(notification.parentHandle);
                                        foreach (OMLPlugin p in plugins)
                                        {
                                            p.OnEntitySpawn(notification.velocity, notification.position, notification.entityID, notification.variant, notification.subtype, parent);
                                            foreach (Item i in p.CustomItemList)
                                                if (hasCustomItem[i.id]) 
                                                    i.OnEntitySpawn(notification.velocity, notification.position, notification.entityID, notification.variant, notification.subtype, parent);
                                        }

                                        new API_EndCall(_OML.Connection).Call();

                                        server.Flush();

                                        // Send response
                                        SpawnEntityEvent_Response response;
                                        response.eventID = _OML.GAME_EVENT_SPAWNENTITY;

                                        ServerOut.Write(RawSerialize(response));
                                    }
                                    else
                                        mw.WriteLine(Level.Warning, "GAME_EVENT_SPAWNENTITY: expected " + SpawnEntityEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    break;

                                case _OML.PLAYER_EVENT_HPUP:
                                    if (bytesLeft + 1 == HpUpEvent_Notification.size())
                                    {
                                        // Receive event

                                        HpUpEvent_Notification notification = RawDeserialize<HpUpEvent_Notification>(ServerIn.ReadBytes(HpUpEvent_Notification.size()), 0);

                                        Player player = new Player(notification.playerHandle);
                                        int tmpAmount = notification.amount;
                                        foreach (OMLPlugin p in plugins)
                                            p.OnPlayerHealthUp(player, ref tmpAmount);

                                        new API_EndCall(_OML.Connection).Call();

                                        server.Flush();

                                        // Send response
                                        HpUpEvent_Response response;
                                        response.eventID = _OML.PLAYER_EVENT_HPUP;
                                        response.amount = tmpAmount;

                                        ServerOut.Write(RawSerialize(response));
                                    }
                                    else
                                        mw.WriteLine(Level.Warning, "PLAYER_EVENT_HPUP: expected " + HpUpEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    break;

                                case _OML.PLAYER_EVENT_HPDOWN:
                                    if (bytesLeft + 1 == HpDownEvent_Notification.size())
                                    {
                                        // Receive event

                                        HpDownEvent_Notification notification = RawDeserialize<HpDownEvent_Notification>(ServerIn.ReadBytes(HpDownEvent_Notification.size()), 0);

                                        Player player = new Player(notification.playerHandle);
                                        int tmpAmount = notification.amount;
                                        foreach (OMLPlugin p in plugins)
                                            p.OnPlayerHealthDown(player, ref tmpAmount);

                                        new API_EndCall(_OML.Connection).Call();

                                        server.Flush();

                                        // Send response
                                        HpUpEvent_Response response;
                                        response.eventID = _OML.PLAYER_EVENT_HPDOWN;
                                        response.amount = tmpAmount;

                                        ServerOut.Write(RawSerialize(response));
                                    }
                                    else
                                        mw.WriteLine(Level.Warning, "PLAYER_EVENT_HPDOWN: expected " + HpDownEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    break;

                                case _OML.PLAYER_EVENT_ADDSOULHEARTS:
                                    if (bytesLeft + 1 == AddSoulHeartsEvent_Notification.size())
                                    {
                                        // Receive event

                                        AddSoulHeartsEvent_Notification notification = RawDeserialize<AddSoulHeartsEvent_Notification>(ServerIn.ReadBytes(AddSoulHeartsEvent_Notification.size()), 0);

                                        Player player = new Player(notification.playerHandle);
                                        int tmpAmount = notification.amount;
                                        foreach (OMLPlugin p in plugins)
                                            p.OnSoulHeartsAdded(player, ref tmpAmount);

                                        new API_EndCall(_OML.Connection).Call();

                                        server.Flush();

                                        // Send response
                                        AddSoulHeartsEvent_Response response;
                                        response.eventID = _OML.PLAYER_EVENT_ADDSOULHEARTS;
                                        response.amount = tmpAmount;

                                        ServerOut.Write(RawSerialize(response));

                                    }
                                    else
                                        mw.WriteLine(Level.Warning, "PLAYER_EVENT_ADDSOULHEARTS: expected " + AddSoulHeartsEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    break;

                                case _OML.ENEMY_EVENT_SHOOTTEARS:
                                    if (bytesLeft + 1 == ShootTearsEvent_Notification.size())
                                    {
                                        // Receive event

                                        ShootTearsEvent_Notification notification = RawDeserialize<ShootTearsEvent_Notification>(ServerIn.ReadBytes(ShootTearsEvent_Notification.size()), 0);

                                        Entity sourceEntity = new Entity(notification.sourceEntityHandle);
                                        foreach (OMLPlugin p in plugins)
                                        {
                                            p.OnEnemyTearShot(notification.velocity, notification.position, sourceEntity, notification.pattern, notification.tearInfo);
                                            foreach (Item i in p.CustomItemList)
                                                if (hasCustomItem[i.id]) 
                                                    i.OnEnemyTearShot(notification.velocity, notification.position, sourceEntity, notification.pattern, notification.tearInfo);
                                        }

                                        new API_EndCall(_OML.Connection).Call();

                                        server.Flush();

                                        // Send response
                                        ShootTearsEvent_Response response;
                                        response.eventID = _OML.ENEMY_EVENT_SHOOTTEARS;

                                        ServerOut.Write(RawSerialize(response));

                                    }
                                    else
                                        mw.WriteLine(Level.Warning, "ENEMY_EVENT_SHOOTTEARS: expected " + ShootTearsEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    break;

                                case _OML.PLAYER_EVENT_CHANGEROOM:
                                    if (bytesLeft + 1 == ChangeRoomEvent_Notification.size())
                                    {
                                        // Receive event

                                        ChangeRoomEvent_Notification notification = RawDeserialize<ChangeRoomEvent_Notification>(ServerIn.ReadBytes(ChangeRoomEvent_Notification.size()), 0);

                                        foreach (OMLPlugin p in plugins)
                                        {
                                            p.OnRoomChange(notification.newRoomIndex);
                                            foreach (Item i in p.CustomItemList)
                                                if (hasCustomItem[i.id]) 
                                                    i.OnRoomChange(notification.newRoomIndex);
                                        }

                                        new API_EndCall(_OML.Connection).Call();

                                        server.Flush();

                                        // Send response
                                        ChangeRoomEvent_Response response;
                                        response.eventID = _OML.ENEMY_EVENT_SHOOTTEARS;

                                        ServerOut.Write(RawSerialize(response));

                                    }
                                    else
                                        mw.WriteLine(Level.Warning, "PLAYER_EVENT_CHANGEROOM: expected " + ChangeRoomEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    break;

                                case _OML.GAME_EVENT_UPDATE:
                                    if (bytesLeft + 1 == GameUpdateEvent_Notification.size())
                                    {
                                        // Receive event
                                        GameUpdateEvent_Notification notification = RawDeserialize<GameUpdateEvent_Notification>(ServerIn.ReadBytes(GameUpdateEvent_Notification.size()), 0);

                                        foreach (OMLPlugin p in plugins)
                                        {
                                            p.OnGameUpdate();
                                            foreach (Item i in p.CustomItemList)
                                                if (hasCustomItem[i.id]) 
                                                    i.OnGameUpdate();
                                        }

                                        new API_EndCall(_OML.Connection).Call();

                                        server.Flush();

                                        // Send response
                                        GameUpdateEvent_Response response;
                                        response.eventID = _OML.GAME_EVENT_UPDATE;

                                        ServerOut.Write(RawSerialize(response));
                                    }
                                    else
                                        mw.WriteLine(Level.Warning, "GAME_EVENT_UPDATE: expected " + GameUpdateEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    break;

                                case _OML.PLAYER_EVENT_UPDATE:
                                    if (bytesLeft + 1 == PlayerUpdateEvent_Notification.size())
                                    {
                                        // Receive event
                                        PlayerUpdateEvent_Notification notification = RawDeserialize<PlayerUpdateEvent_Notification>(ServerIn.ReadBytes(PlayerUpdateEvent_Notification.size()), 0);

                                        Player player = new Player(notification.playerHandle);
                                        foreach (OMLPlugin p in plugins)
                                        {
                                            p.OnPlayerUpdate(player);
                                            foreach (Item i in p.CustomItemList)
                                                if (hasCustomItem[i.id]) 
                                                    i.OnPlayerUpdate(player);
                                        }

                                        // "Surprise" calls
                                        for (int i = 0; i < commandQueue.Count; i++)
                                        {
                                            Command c = new Command(new Action<object[]>(Wrappers.Empty_Wrapper), new object[0]);
                                            commandQueue.TryDequeue(out c);
                                            if (c._params[0].GetType() != typeof(Player))
                                                c.func(c._params);
                                            else
                                            {
                                                c._params[0] = player;
                                                c.func(c._params);
                                            }
                                        }

                                        new API_EndCall(_OML.Connection).Call();

                                        server.Flush();

                                        // Send response
                                        PlayerUpdateEvent_Response response;
                                        response.eventID = _OML.GAME_EVENT_UPDATE;

                                        ServerOut.Write(RawSerialize(response));
                                    }
                                    else
                                        mw.WriteLine(Level.Warning, "PLAYER_EVENT_UPDATE: expected " + PlayerUpdateEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    break;

                                case _OML.PLAYER_EVENT_USECARD:
                                    if (bytesLeft + 1 == UseCardEvent_Notification.size())
                                    {
                                        // Receive event

                                        UseCardEvent_Notification notification = RawDeserialize<UseCardEvent_Notification>(ServerIn.ReadBytes(UseCardEvent_Notification.size()), 0);

                                        Player player = new Player(notification.playerHandle);
                                        bool handled = false;

                                        foreach (OMLPlugin p in plugins)
                                        {
                                            p.OnPlayerCardUse(player, notification.cardID, ref handled);
                                            foreach (Item i in p.CustomItemList)
                                                if (hasCustomItem[i.id]) 
                                                    i.OnPlayerCardUse(player, notification.cardID, ref handled);
                                        }

                                        new API_EndCall(_OML.Connection).Call();

                                        server.Flush();

                                        // Send response
                                        UseCardEvent_Response response;
                                        response.eventID = _OML.PLAYER_EVENT_USECARD;
                                        response.handled = handled;

                                        ServerOut.Write(RawSerialize(response));

                                    }
                                    else
                                        mw.WriteLine(Level.Warning, "PLAYER_EVENT_USECARD: expected " + UseCardEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    break;

                                case _OML.GAME_EVENT_GOTOFLOOR:
                                    if (bytesLeft + 1 == GotoFloorEvent_Notification.size())
                                    {
                                        // Receive event

                                        GotoFloorEvent_Notification notification = RawDeserialize<GotoFloorEvent_Notification>(ServerIn.ReadBytes(GotoFloorEvent_Notification.size()), 0);

                                        foreach (OMLPlugin p in plugins)
                                        {
                                            p.OnGotoFloor((Floor)notification.nextFloorNo);
                                            foreach (Item i in p.CustomItemList)
                                                if (hasCustomItem[i.id]) 
                                                    i.OnGotoFloor((Floor)notification.nextFloorNo);
                                        }

                                        new API_EndCall(_OML.Connection).Call();

                                        server.Flush();

                                        // Send response
                                        GotoFloorEvent_Response response;
                                        response.eventID = _OML.GAME_EVENT_GOTOFLOOR;

                                        ServerOut.Write(RawSerialize(response));

                                    }
                                    else
                                        mw.WriteLine(Level.Warning, "GAME_EVENT_GOTOFLOOR: expected " + GotoFloorEvent_Response.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    break;
                                case _OML.PLAYER_EVENT_HITBYENEMY:
                                    if (bytesLeft + 1 == PlayerGetsHit_Notification.size())
                                    {
                                        PlayerGetsHit_Notification notification = RawDeserialize<PlayerGetsHit_Notification>(ServerIn.ReadBytes(PlayerGetsHit_Notification.size()), 0);

                                        Player pl = new Player(notification.player);
                                        //Entity e = new Entity(notification.enemy);
                                        

                                        foreach(OMLPlugin p in plugins)
                                            foreach (Item i in p.CustomItemList)
                                                if (hasCustomItem[i.id])
                                                    i.OnEnemyContact(/*notification.a1, notification.a2, notification.damage, notification.a4, e,*/ pl);

                                        new API_EndCall(_OML.Connection).Call();

                                        server.Flush();

                                        PlayerGetsHit_Response response;
                                        response.eventID = _OML.PLAYER_EVENT_HITBYENEMY;

                                        ServerOut.Write(RawSerialize(response));
                                    }
                                    break;
                            }

                            if (server.IsConnected)
                                server.Flush();
                        }
                    }
                    catch (IOException)
                    {
                        mw.WriteLine(Level.Error, "pipe error occured.");
                        return;
                    }
                    //catch (Exception ex)
                    //{
                    //    mw.WriteLine(Level.Error, "Fatal error.");
                    //    mw.WriteLine(Level.Error, ex.Message);
                    //    mw.WriteLine(Level.Error, ex.Source);
                    //    mw.WriteLine(Level.Error, ex.StackTrace);
                    //    throw;
                    //}
                }
            }
        }

        static byte[] GetBytes(string str, int len)
        {
            byte[] bytes = new byte[len];
            Buffer.BlockCopy(str.ToCharArray(), 0, bytes, 0, bytes.Length);
            return bytes;
        }

        public static T RawDeserialize<T>(byte[] rawData, int position)
        {
            int rawsize = Marshal.SizeOf(typeof(T));
            if (rawsize > rawData.Length - position)
                throw new ArgumentException("Not enough data to fill struct. Array length from position: " + (rawData.Length - position) + ", Struct length: " + rawsize);
            IntPtr buffer = Marshal.AllocHGlobal(rawsize);
            Marshal.Copy(rawData, position, buffer, rawsize);
            T retobj = (T)Marshal.PtrToStructure(buffer, typeof(T));
            Marshal.FreeHGlobal(buffer);
            return retobj;
        }
        public static byte[] RawSerialize(object anything)
        {
            int rawSize = Marshal.SizeOf(anything);
            IntPtr buffer = Marshal.AllocHGlobal(rawSize);
            Marshal.StructureToPtr(anything, buffer, false);
            byte[] rawDatas = new byte[rawSize];
            Marshal.Copy(buffer, rawDatas, 0, rawSize);
            Marshal.FreeHGlobal(buffer);
            return rawDatas;
        }
    }
    public class Command
    {
        public Action<object[]> func;
        public object[] _params;

        public Command(Action<object[]> func, object[] _params)
        {
            this.func = func;
            this._params = _params;
        }
    }
}
