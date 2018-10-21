using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MPS
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
            
        }

        private void MainForm_Shown(object sender, EventArgs e)
        {
            try
            {
                ConSerialPort.Connect();
                if (ConSerialPort.connected)
                {
                    Thread.Sleep(2000);
                    WorkForm wf = new WorkForm();
                    wf.Show();
                    this.Hide();

                }
                else
                {
                    label.Text = "Не удалюсь подключиться, перезапустите программу.";
                }
            }
            catch
            {
                MessageBox.Show("Возникла ошибка", "Сохранение расписания", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
