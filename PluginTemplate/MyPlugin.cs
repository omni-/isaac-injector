using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OML;

namespace PluginTemplate
{
    public class MyPlugin : OMLPlugin
    {
        public MyPlugin() 
        {
            PluginName = "TestPlugin";
            PluginVersion = "1.1";
            PluginAuthor = "omni";
        }
        public override void PluginInit()
        {
            Commands.Add("echo", new Command(MyCallback, "", new List<Type>() { typeof(string) }));
            base.PluginInit();
        }
        public override void OnPlayerAddCollectible(Player player, int a2, int id, int a4)
        {
            player.SetStat(PlayerStat.Luck, 2);

            base.OnPlayerAddCollectible(player, a2, id, a4);
        }
        public void MyCallback(object[] args)
        {
            Console.WriteLine(args[0]);
        }
    }
}
