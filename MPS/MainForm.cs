using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace MPS
{
    public partial class MainForm : Form
    {
        private delegate void updateDelegate(string txt);
        public MainForm()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Подключение к Arduino
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void connectButton_Click(object sender, EventArgs e)
        {
            try
            {
                ConSerialPort.connect();
                if (ConSerialPort.connected)
                {
                    connectButton.Enabled = false;
                    disconnectButton.Enabled = true;
                }
                else
                {
                    MessageBox.Show("Возникла ошибка подключения", "Сохранение расписания", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                
            }
            catch
            {
                MessageBox.Show("Возникла ошибка2", "Сохранение расписания", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// Открытие формы добавления расписаний
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void AddButton_Click(object sender, EventArgs e)
        {
          //  if (ConSerialPort.connected)
         //   {
                Form add = new AddForm();
                add.Left = this.Left;
                add.Top = this.Top;
                add.Show();
                this.Hide();
        //    }
        //    else
         //       MessageBox.Show("Arduino не подключен", "Добавление расписания", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        /// <summary>
        /// Открытие формы с полным расписанием
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ViewButton_Click(object sender, EventArgs e)
        {
           // if (ConSerialPort.connected)
          //  {
                Form view = new ViewForm();
                view.Left = this.Left;
                view.Top = this.Top;
                view.Show();
                this.Hide();
        //    }
         //   else
         //       MessageBox.Show("Arduino не подключен", "Полное расписание", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }



        private void disconnectButton_Click(object sender, EventArgs e)
        {
            ConSerialPort.disconnect();
            if (!ConSerialPort.connected)
            {
                connectButton.Enabled = true;
                disconnectButton.Enabled = false;
            }
            else
            {
                MessageBox.Show("Возникла ошибка при оиключении", "Отключение Arduino", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
