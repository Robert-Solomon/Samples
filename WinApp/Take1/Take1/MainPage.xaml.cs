using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace Take1
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            content.Text = "Obladi";
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            content.Text = "Oblada";
        }

        private void Button_Click_4(object sender, RoutedEventArgs e)
        {
            content.Text = "Life Goes On";
        }

        private void Button_Click_5(object sender, RoutedEventArgs e)
        {
            content.Text = "Bla Bla Bla";
        }

        private void Button_Click_6(object sender, RoutedEventArgs e)
        {
            content.Text = "Oh How Life Goes";
        }

        private void Button_Click_7(object sender, RoutedEventArgs e)
        {
            content.Text = "On Bla.";
        }
    }
}
