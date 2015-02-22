using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Xml;
using System.Xml.Linq;

namespace OML
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public const string version = "v1.1";

        private string Path;

        public string path
        {
            get { return Path; }
            set
            {
                Path = value;
                using (StreamWriter s = new StreamWriter(".usersettings"))
                {
                    s.WriteLine("false");
                    s.WriteLine(path);
                }
            }
        }

        public Handler h;

        public ObservableCollection<DisplayItem> ItemList = new ObservableCollection<DisplayItem>();

        public ObservableCollection<DisplayItem> enemylist = new ObservableCollection<DisplayItem>();

        public List<DisplayItem> pickuplist = new List<DisplayItem>();

        public List<DisplayItem> otherlist = new List<DisplayItem>();

        public List<Card> cardlist = new List<Card>();

        public List<Card> pilllist = new List<Card>();

        public MainWindow()
        {
            AppDomain currentDomain = AppDomain.CurrentDomain;
            currentDomain.UnhandledException += new UnhandledExceptionEventHandler(AppDomainException);
            InitializeComponent();
            Init();
            var list = new ObservableCollection<DisplayItem>(ItemList.OrderBy(x => x.ID));
            itemBox.ItemsSource = list;

            pcBox.Items.Clear();
            List<DisplayItem> tlist = new List<DisplayItem> { new DisplayItem(new BitmapImage(new Uri(System.IO.Path.GetFullPath("res\\pillcard\\card.png"))), "card", 0), 
                                                              new DisplayItem(new BitmapImage(new Uri(System.IO.Path.GetFullPath("res\\pillcard\\pill.png"))), "pill", 0) };
            pcBox.ItemsSource = tlist;
            
            pcSelectBox.ItemsSource = cardlist;

            spBox.ItemsSource = pickuplist;

            enemyBox.ItemsSource = enemylist;

            otherBox.ItemsSource = otherlist;
        }

        public void Init()
        {
            Title = "omni's modloader - " + version;
            _OML.Init();
            for (int i = 0; i < Directory.GetFiles("res\\items", "*.png").Count(); i++)
            {
                string file = Directory.GetFiles("res\\items", "*.png")[i];
                string temp = file;
                temp = temp.Replace("res\\items\\", "");
                temp = temp.Replace("-", "");
                temp = new string(temp.Skip(6).ToArray()).Trim();
                var regex = new Regex(Regex.Escape(" "));
                temp = regex.Replace(temp, "", 2);
                BitmapImage image = new BitmapImage(new Uri(System.IO.Path.GetFullPath(file)));
                int result = 0;
                int itr = 3;
                while (result == 0)
                {
                    string att = new string(temp.Take(itr).ToArray());
                    int.TryParse(att, out result);
                    if (result == 0)
                        itr--;
                }
                string name = new string(temp.Skip(itr).ToArray()).Replace(".png", "");
                ItemList.Add(new DisplayItem(image, name, result));
            }
            XDocument xdoc = XDocument.Load("res\\xml\\pocketitems.xml");
            foreach (var element in xdoc.Elements("pocketitems").Elements("card").Skip(1))
                cardlist.Add(new Card(element.Attribute("name").Value, int.Parse(element.Attribute("id").Value)));
            xdoc = XDocument.Load("res\\xml\\entities2.xml");
            var collection = xdoc.Elements("entities").Elements("entity");
            foreach (var element in collection.Where(x => x.Attribute("name").Value.StartsWith("Pill ")))
                pilllist.Add(new Card(element.Attribute("name").Value.Replace("Pill ", ""), int.Parse(element.Attribute("subtype").Value)));
            foreach(string file in Directory.GetFiles("res\\pickups", "*.png"))
            {
                string[] namesplit = file.Split(new char[] { ' ' }, 3);
                namesplit[0] = namesplit[0].Replace("res\\pickups\\", "");
                int[] myInts = Array.ConvertAll(namesplit[0].Split('.'), int.Parse);
                pickuplist.Add(new DisplayItem(new BitmapImage(new Uri(System.IO.Path.GetFullPath(file))), namesplit[2].Replace(".png", ""), myInts[0], myInts[1], myInts[2]));
            }
            foreach (string file in Directory.GetFiles("res\\other", "*.png"))
            {
                string[] namesplit = file.Split(new char[] { ' ' }, 3);
                namesplit[0] = namesplit[0].Replace("res\\other\\", "");
                int[] myInts = Array.ConvertAll(namesplit[0].Split('.'), int.Parse);
                otherlist.Add(new DisplayItem(new BitmapImage(new Uri(System.IO.Path.GetFullPath(file))), namesplit[2].Replace(".png", ""), myInts[0], myInts[1], myInts[2]));
            }
            foreach (string file in Directory.GetFiles("res\\enemy", "*.png"))
            {
                string[] namesplit = file.Split(new char[] { ' ' }, 3);
                namesplit[0] = namesplit[0].Replace("res\\enemy\\", "");
                int[] myInts = Array.ConvertAll(namesplit[0].Split('.'), int.Parse);
                enemylist.Add(new DisplayItem(new BitmapImage(new Uri(System.IO.Path.GetFullPath(file))), namesplit[2].Replace(".png", ""), myInts[0], myInts[1], myInts[2]));
            }
        }
        public void WriteLine(Level l, string text, params object[] args)
        {
            this.Dispatcher.Invoke((Action)(() =>
            {
                switch (l)
                {
                    case Level.Info:
                        launchBlock.Foreground = Brushes.White;
                        launchBlock.Text += "[i] ";
                        launchBlock.Foreground = Brushes.Black;
                        launchBlock.Text += String.Format(text, args) + "\r\n";
                        break;
                    case Level.Warning:
                        launchBlock.Foreground = Brushes.Gold;
                        launchBlock.Text += "[w] ";
                        launchBlock.Foreground = Brushes.Black;
                        launchBlock.Text += String.Format(text, args) + "\r\n";
                        break;
                    case Level.Error:
                        launchBlock.Foreground = Brushes.Blue;
                        launchBlock.Text += "[e] ";
                        launchBlock.Foreground = Brushes.Black;
                        launchBlock.Text += String.Format(text, args) + "\r\n";
                        break;
                }
            }));
        }

        private void AppDomainException(object sender, UnhandledExceptionEventArgs e)
        {
            File.OpenWrite("crashlog.txt").Close();
            using (StreamWriter writer = new StreamWriter("crashlog.txt", true))
            {
                Exception ex = (Exception)e.ExceptionObject;
                writer.WriteLine("==========EXCEPTION==========");
                writer.WriteLine("Message: " + ex.Message);
                writer.WriteLine("Inner exception: " + ex.InnerException);
                writer.WriteLine("Source: " + ex.Source);
                writer.WriteLine("Target site: " + ex.TargetSite);
                writer.WriteLine("Stack trace: " + ex.StackTrace);
            }
        }
        private void launchButton_Click(object sender, RoutedEventArgs e)
        {
            WriteLine(Level.Info, "Initializing...");
            string tpath = "";
            bool result = Loader.init(ref tpath);
            path = tpath;
            if (Loader.FirstTimeFlag)
            {
                PopupWindow p = new PopupWindow(this);
                p.ShowDialog();
            }
            try
            {
                File.Copy("res\\dll\\dsound.dll", path + "\\dsound.dll", true);
            }
            catch (IOException) { }
            launchBlock.Inlines.Remove(init);
            if (!result)
            {
                WriteLine(Level.Error, "Loader init failed.");
                return;
            }
            h = new Handler(this);
            WriteLine(Level.Info, "Starting game...");
            Process proc;
            if (path.Contains("steam") || path.Contains("Steam"))
                proc = Process.Start("steam://rungameid/250900");
            else
                proc = Process.Start(path + "\\isaac-ng.exe");
            WriteLine(Level.Info, "Awaiting connection...");
            Thread t = new Thread(new ThreadStart(h.Handle));
            t.IsBackground = true;
            t.Start();
            controlGrid.IsEnabled = true;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            statBox.ItemsSource = Enum.GetValues(typeof(BoxPlayerStat)).Cast<BoxPlayerStat>();
            jumpBox.ItemsSource = Enum.GetValues(typeof(Floor)).Cast<Floor>();
        }

        private void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            statText.Text = "";
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (itemBox.SelectedItem != null)
                h.commandQueue.Enqueue(new Command(Wrappers.SpawnItem_Wrapper, new object[] { new Player(IntPtr.Zero), ((DisplayItem)itemBox.SelectedItem).ID }));
            int result = 0;
            if (int.TryParse(itemIDBox.Text, out result))
                h.commandQueue.Enqueue(new Command(Wrappers.SpawnItem_Wrapper, new object[] { new Player(IntPtr.Zero), result }));
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            int result = 0;
            int.TryParse(roomBox.Text, out result);
            if (result != 0)
                h.commandQueue.Enqueue(new Command(Wrappers.Teleport_Wrapper, new object[] { int.Parse(roomBox.Text) }));
        }

        private void statText_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                statBox.Focus();
            }
        }

        private void statText_LostFocus(object sender, RoutedEventArgs e)
        {
            if (statText.Text == "")
                statText.Text = "Value";
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            if (jumpBox.SelectedItem !=  null)
                h.commandQueue.Enqueue(new Command(Wrappers.JumpFloor_Wrapper, new object[] { (Floor)jumpBox.SelectedItem }));
        }

        private void roomBox_GotFocus(object sender, RoutedEventArgs e)
        {
            roomBox.Text = "";
        }

        #region old
        //private void idBox_GotFocus(object sender, RoutedEventArgs e)
        //{
        //    idBox.Text = "";
        //}

        //private void variantBox_GotFocus(object sender, RoutedEventArgs e)
        //{
        //    variantBox.Text = "";
        //}

        //private void subtypeBox_GotFocus(object sender, RoutedEventArgs e)
        //{
        //    subtypeBox.Text = "";
        //}

        //private void Button_Click_3(object sender, RoutedEventArgs e)
        //{
        //    int id = -1;
        //    int.TryParse(idBox.Text, out id);
        //    int variant = -1;
        //    int.TryParse(variantBox.Text, out variant);
        //    int subtype = -1;
        //    int.TryParse(subtypeBox.Text, out subtype);
        //    float x = -1.0f;
        //    float.TryParse(xBox.Text, out x);
        //    float y = -1.0f;
        //    float.TryParse(yBox.Text, out y);
        //    if (id != -1 && variant != -1 && subtype != -1 && x != -1.0f && y != -1.0f)
        //        h.commandQueue.Enqueue(new Command(Wrappers.SpawnEntity_Wrapper, new object[] { id, variant, subtype, x, y }));
        //}
        #endregion

        private void statButton_Click(object sender, RoutedEventArgs e)
        {
            int result = -1;
            int.TryParse(statText.Text, out result);
            if (result != -1)
            {
                switch ((BoxPlayerStat)statBox.SelectedItem)
                {
                    case BoxPlayerStat.Coins:
                        h.commandQueue.Enqueue(new Command(Wrappers.SetInv_Wrapper, new object[] { new Player(IntPtr.Zero), PlayerInv.Coins, result }));
                        break;
                    case BoxPlayerStat.Bombs:
                        h.commandQueue.Enqueue(new Command(Wrappers.SetInv_Wrapper, new object[] { new Player(IntPtr.Zero), PlayerInv.Bombs, result }));
                        break;
                    case BoxPlayerStat.Keys:
                        h.commandQueue.Enqueue(new Command(Wrappers.SetInv_Wrapper, new object[] { new Player(IntPtr.Zero), PlayerInv.Keys, result }));
                        break;
                    case BoxPlayerStat.Damage:
                        h.commandQueue.Enqueue(new Command(Wrappers.SetStat_Wrapper, new object[] { new Player(IntPtr.Zero), PlayerStat.Damage, result }));
                        break;
                    case BoxPlayerStat.Firerate:
                        h.commandQueue.Enqueue(new Command(Wrappers.SetStat_Wrapper, new object[] { new Player(IntPtr.Zero), PlayerStat.FireRate, result }));
                        break;
                    case BoxPlayerStat.Luck:
                        h.commandQueue.Enqueue(new Command(Wrappers.SetStat_Wrapper, new object[] { new Player(IntPtr.Zero), PlayerStat.Luck, result }));
                        break;
                    case BoxPlayerStat.Range:
                        h.commandQueue.Enqueue(new Command(Wrappers.SetStat_Wrapper, new object[] { new Player(IntPtr.Zero), PlayerStat.Range, result }));
                        break;
                    case BoxPlayerStat.Shotspeed:
                        h.commandQueue.Enqueue(new Command(Wrappers.SetStat_Wrapper, new object[] { new Player(IntPtr.Zero), PlayerStat.ShotSpeed, result }));
                        break;
                    case BoxPlayerStat.Speed:
                        h.commandQueue.Enqueue(new Command(Wrappers.SetStat_Wrapper, new object[] { new Player(IntPtr.Zero), PlayerStat.Speed, result }));
                        break;
                }
            }
        }

        private void itemBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            
        }

        private void itemIDBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            int result = 0;
            if (int.TryParse(itemIDBox.Text, out result))
                itemBox.SelectedItem = ItemList.FirstOrDefault(d => d.ID.ToString().StartsWith(itemIDBox.Text));
            else
            {
                itemBox.SelectedItem = ItemList.FirstOrDefault(d => d.Name.ToLower().StartsWith(itemIDBox.Text.ToLower()));
                if (itemBox.SelectedItem == null)
                    itemBox.SelectedItem = ItemList.FirstOrDefault(d => d.Name.ToLower().Contains(itemIDBox.Text.ToLower()));
            }
        }

        private void itemIDBox_GotFocus(object sender, RoutedEventArgs e)
        {
            itemIDBox.Text = "";
        }

        private void itemIDBox_LostFocus(object sender, RoutedEventArgs e)
        {
            if (itemIDBox.Text == "")
                itemIDBox.Text = "Search ID";
        }

        private void roomBox_LostFocus(object sender, RoutedEventArgs e)
        {
            if (roomBox.Text == "")
                roomBox.Text = "Room number";
        }

        #region old
        //private void idBox_LostFocus(object sender, RoutedEventArgs e)
        //{
        //    if (idBox.Text == "")
        //        idBox.Text = "ID";
        //}

        //private void variantBox_LostFocus(object sender, RoutedEventArgs e)
        //{
        //    if (variantBox.Text == "")
        //        variantBox.Text = "Variant";
        //}

        //private void subtypeBox_LostFocus(object sender, RoutedEventArgs e)
        //{
        //    if (subtypeBox.Text == "")
        //        subtypeBox.Text = "Subtype";
        //}
        #endregion
        private void xBox_LostFocus(object sender, RoutedEventArgs e)
        {
            if (xBox.Text == "")
                xBox.Text = "X Coordinate";
        }

        private void yBox_LostFocus(object sender, RoutedEventArgs e)
        {
            if (yBox.Text == "")
                yBox.Text = "Y Coordinate";
        }

        private void pcBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (pcBox.SelectedIndex == 0)
                pcSelectBox.ItemsSource = cardlist;
            else if (pcBox.SelectedIndex == 1)
                pcSelectBox.ItemsSource = pilllist;
        }

        private void pcButton_Click(object sender, RoutedEventArgs e)
        {
            if (((DisplayItem)pcBox.SelectedItem).Name == "card")
                API.SpawnEntity(5, 300, ((Card)pcSelectBox.SelectedItem).Id, getx(), gety());
            else if (((DisplayItem)pcBox.SelectedItem).Name == "pill")
                API.SpawnEntity(5, 70, ((Card)pcSelectBox.SelectedItem).Id, getx(), gety());
        }

        private void spButton_Click(object sender, RoutedEventArgs e)
        {
            if (spBox.SelectedItem != null)
            {
                DisplayItem item = (DisplayItem)spBox.SelectedItem;
                if (item.Variant != 10 || item.Subtype != 0)
                    API.SpawnEntity(item.ID, item.Variant, item.Subtype, getx(), gety());
                else
                {
                    DisplayItem i = pickuplist[new Random().Next(0, pickuplist.Count - 1)];
                    API.SpawnEntity(i.ID, i.Variant, i.Subtype, getx(), gety());
                }
            }
        }

        private void enemyIDBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (enemyBox != null)
            {
                int result = 0;
                if (int.TryParse(enemyIDBox.Text, out result))
                    enemyBox.SelectedItem = enemylist.FirstOrDefault(d => d.ID.ToString().StartsWith(enemyIDBox.Text));
                else
                {
                    enemyBox.SelectedItem = enemylist.FirstOrDefault(d => d.Name.ToLower().StartsWith(enemyIDBox.Text.ToLower()));
                    if (enemyBox.SelectedItem == null)
                        enemyBox.SelectedItem = enemylist.FirstOrDefault(d => d.Name.ToLower().Contains(enemyIDBox.Text.ToLower()));
                }
            }
        }

        private void enemyIDBox_GotFocus(object sender, RoutedEventArgs e)
        {
            enemyIDBox.Text = "";
        }

        private void enemyIDBox_LostFocus(object sender, RoutedEventArgs e)
        {
            if (enemyIDBox.Text == "")
                enemyIDBox.Text = "Search";
        }

        private void enemyButton_Click(object sender, RoutedEventArgs e)
        {
            if (enemyBox != null && enemyBox.SelectedItem != null)
            {
                DisplayItem i = (DisplayItem)enemyBox.SelectedItem;
                API.SpawnEntity(i.ID, i.Variant, i.Subtype, getx(), gety());
            }
        }

        private void otherButton_Click(object sender, RoutedEventArgs e)
        {
            if (otherBox != null && otherBox.SelectedItem != null)
            {
                DisplayItem i = (DisplayItem)otherBox.SelectedItem;
                API.SpawnEntity(i.ID, i.Variant, i.Subtype, getx(), gety());
            }
        }
        public int getx()
        {
            int ret = 3;
            if (!int.TryParse(xBox.Text, out ret))
                WriteLine(Level.Warning, "invalid position. spawning at default (3, 3)");
            return ret;
        }
        public int gety()
        {
            int ret = 3;
            if (!int.TryParse(yBox.Text, out ret))
                WriteLine(Level.Warning, "invalid position. spawning at default (3, 3)");
            return ret;
        }

        private void yBox_GotFocus(object sender, RoutedEventArgs e)
        {
            yBox.Text = "";
        }

        private void xBox_GotFocus(object sender, RoutedEventArgs e)
        {
            xBox.Text = "";
        }
    }
    public enum Level
    {
        Info,
        Warning,
        Error
    }
    public enum BoxPlayerStat
    {
        Coins,
        Bombs,
        Keys,
        Damage,
        Speed,
        Firerate,
        Shotspeed,
        Range,
        Luck
    }
    public class Card
    {
        public string Name { get; set; }
        public int Id { get; set; }

        public Card(string name, int id)
        {
            Name = name;
            Id = id;
        }
    }
    public class DisplayItem : IComparable
    {
        public BitmapImage Image { get; set; }
        public string Name { get; set; }
        public int ID { get; set; }
        public int Variant { get; set; }
        public int Subtype { get; set; }
        public DisplayItem(BitmapImage image, string name, int number)
        {
            Image = image;
            Name = name;
            ID = number;
        }
        public DisplayItem(BitmapImage image, string name, int number, int variant, int subtype)
        {
            Image = image;
            Name = name;
            ID = number;
            Variant = variant;
            Subtype = subtype;
        }
        int IComparable.CompareTo(object obj)
        {
            DisplayItem d = (DisplayItem)obj;
            if (ID > d.ID)
                return 1;
            if (ID < d.ID)
                return -1;
            else
                return 0;
        }
    }
}
