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

        public Handler h;

        private ObservableCollection<DisplayItem> ItemList = new ObservableCollection<DisplayItem>();

        public MainWindow()
        {
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
        public void Write(string text, params object[] args)
        {
            this.Dispatcher.Invoke((Action)(() =>
            {
                launchBlock.Text += String.Format(text, args);
            }));
        }

        public void WriteLine(string text, params object[] args)
        {
            this.Dispatcher.Invoke((Action)(() =>
            {
                launchBlock.Text += String.Format(text + "\r\n", args);
            }));
        }

        private void launchButton_Click(object sender, RoutedEventArgs e)
        {
            WriteLine("Initializing...");
            bool result = Loader.init();
            launchBlock.Inlines.Remove(init);
            if (!result)
            {
                WriteLine("Loader init failed.");
                return;
            }
            h = new Handler(this);
            WriteLine("Starting game...");
            Process proc = Process.Start("steam://rungameid/250900");
            WriteLine("Awaiting connection...");
            Thread t = new Thread(new ThreadStart(h.Handle));
            t.Start();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            statBox.ItemsSource = Enum.GetValues(typeof(BoxPlayerStat)).Cast<BoxPlayerStat>();
        }

        private void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            statText.Text = "";
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (itemBox.SelectedItem != null)
                h.commandQueue.Enqueue(new Command(Wrappers.SpawnItem_Wrapper, new object[] { ((DisplayItem)itemBox.SelectedItem).Number }));
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            int result = 0;
            int.TryParse(roomBox.Text, out result);
            if (result != 0)
                h.commandQueue.Enqueue(new Command(Wrappers.Teleport_Wrapper, new object[] { int.Parse(roomBox.Text) }));
        }
        string ReplaceFirst(string text, string search, string replace)
        {
            int pos = text.IndexOf(search);
            if (pos < 0)
            {
                return text;
            }
            return text.Substring(0, pos) + replace + text.Substring(pos + search.Length);
        }
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
