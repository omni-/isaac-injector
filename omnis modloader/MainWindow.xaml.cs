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

namespace OML
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public const string version = "v1.0";

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

        private ObservableCollection<DisplayItem> ItemList = new ObservableCollection<DisplayItem>();

        public MainWindow()
        {
            AppDomain currentDomain = AppDomain.CurrentDomain;
            currentDomain.UnhandledException += new UnhandledExceptionEventHandler(AppDomainException);
            InitializeComponent();
            Init();
            var list = new ObservableCollection<DisplayItem>(ItemList.OrderBy(x => x.Number));
            itemBox.ItemsSource = list;
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
            File.Copy("res\\dll\\dsound.dll", path + "\\dsound.dll", true);
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
                h.commandQueue.Enqueue(new Command(Wrappers.SpawnItem_Wrapper, new object[] { new Player(IntPtr.Zero), ((DisplayItem)itemBox.SelectedItem).Number }));
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

        private void idBox_GotFocus(object sender, RoutedEventArgs e)
        {
            idBox.Text = "";
        }

        private void variantBox_GotFocus(object sender, RoutedEventArgs e)
        {
            variantBox.Text = "";
        }

        private void subtypeBox_GotFocus(object sender, RoutedEventArgs e)
        {
            subtypeBox.Text = "";
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            int id = -1;
            int.TryParse(idBox.Text, out id);
            int variant = -1;
            int.TryParse(variantBox.Text, out variant);
            int subtype = -1;
            int.TryParse(subtypeBox.Text, out subtype);
            float x = -1.0f;
            float.TryParse(xBox.Text, out x);
            float y = -1.0f;
            float.TryParse(yBox.Text, out y);
            if (id != -1 && variant != -1 && subtype != -1 && x != -1.0f && y != -1.0f)
                h.commandQueue.Enqueue(new Command(Wrappers.SpawnEntity_Wrapper, new object[] { id, variant, subtype, x, y }));
        }

        private void xBox_GotFocus(object sender, RoutedEventArgs e)
        {
            xBox.Text = "";
        }

        private void yBox_GotFocus(object sender, RoutedEventArgs e)
        {
            yBox.Text = "";
        }

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

        private void itemBox_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            foreach(DisplayItem d in itemBox.Items)
            {
                if (d.Name.Contains(e.Text) || d.Number.ToString().Contains(e.Text))
                    itemBox.SelectedItem = d;
            }
            //e.Handled = true;
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
    public class DisplayItem : IComparable
    {
        public BitmapImage Image { get; set; }
        public string Name { get; set; }
        public int Number { get; set; }
        public DisplayItem(BitmapImage image, string name, int number)
        {
            Image = image;
            Name = name;
            Number = number;
        }
        int IComparable.CompareTo(object obj)
        {
            DisplayItem d = (DisplayItem)obj;
            if (Number > d.Number)
                return 1;
            if (Number < d.Number)
                return -1;
            else
                return 0;
        }
    }
}
