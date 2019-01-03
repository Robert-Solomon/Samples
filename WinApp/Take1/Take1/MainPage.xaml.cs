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

        private void Button_CreateExercise_Click(object sender, RoutedEventArgs e)
        {
            content.Text = "Obladi";
        }

        private void Button_DisplayExerciseList_Click(object sender, RoutedEventArgs e)
        {
            content.Text = "Oblada";
        }

        private void Button_CreateWorkout_Click(object sender, RoutedEventArgs e)
        {
            content.Text = "Life Goes On";
        }

        private void Button_SaveChanges_Click(object sender, RoutedEventArgs e)
        {
            content.Text = "Bla Bla Bla";
        }

        private void Button_DiscardChanges_Click(object sender, RoutedEventArgs e)
        {
            content.Text = "Oh How Life Goes";
        }

        private void Button_Settings_Click(object sender, RoutedEventArgs e)
        {
            content.Text = "On Bla.";
        }
    }
}
