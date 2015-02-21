using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
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
            Item item = new TheSun();
            item.Name = "The Sun";
            item.resource = new OMLResource(ResourceType.Item, "MyPlugin\\sun.png", "sun.png");
            item.Type = ItemType.passive;
            item.Pool = ItemPool.treasure;
            item.DecreaseBy = 1;
            item.PickupText = "Brighten your day!";
            item.Weight = 1;
            item.RemoveOn = .1f;
            CustomItemList.Add(item);
            item = new TheMoon();
            item.Name = "The Moon";
            item.resource = new OMLResource(ResourceType.Item, "MyPlugin\\moon.png", "moon.png");
            item.Type = ItemType.passive;
            item.Pool = ItemPool.treasure;
            item.DecreaseBy = 1;
            item.PickupText = "Brighten your night!";
            item.Weight = 1;
            item.RemoveOn = .1f;
            CustomItemList.Add(item);
        }
        public override void PluginInit()
        {
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
    
    public class TheSun : Item
    {
        public override void OnEnemyContact(/*int a1, int a2, float damage, int a4, Entity enemy,*/ Player p)
        {
            p.Keys++;
            base.OnEnemyContact(/*a1, a2, damage, a4, enemy,*/ p);
        }

        public override void OnPickup(Player player)
        {
            player.Keys += 11;
            base.OnPickup(player);
        }
    }

    public class TheMoon : Item
    {
        public override void OnEnemyContact(/*int a1, int a2, float damage, int a4, Entity enemy,*/ Player p)
        {
            p.Bombs++;
            base.OnEnemyContact(/*a1, a2, damage, a4, enemy,*/ p);
        }
    }
}
