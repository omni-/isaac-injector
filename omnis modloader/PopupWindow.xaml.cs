using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace OML
{
    /// <summary>
    /// Interaction logic for PopupWindow.xaml
    /// </summary>
    public partial class PopupWindow : Window
    {
        MainWindow mw;
        bool has = false;
        public PopupWindow(MainWindow mw)
        {
            InitializeComponent();
            this.mw = mw;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            bool exists = Directory.Exists(box.Text);
            if (box.Text == null || box.Text == "" || !exists)
                return;
            mw.path = box.Text;
            has = true;
            Close();
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (!has)
                e.Cancel = true;
        }
    }
}
