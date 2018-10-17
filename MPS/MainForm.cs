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
        private void ConnectButton_Click(object sender, EventArgs e)
        {
            try
            {
                ConSerialPort.Connect();
                if (ConSerialPort.connected)
                {
                    connectButton.Enabled = false;
                    disconnectButton.Enabled = true;
                }
                //else
                //{
                //    MessageBox.Show("Возникла ошибка подключения", "Сохранение расписания", MessageBoxButtons.OK, MessageBoxIcon.Error);
                //}

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
            if (ConSerialPort.connected)
            {
                Form add = new AddForm
                {
                    Left = this.Left,
                    Top = this.Top
                };
                add.Show();
                this.Hide();
            }
            else
                MessageBox.Show("Arduino не подключен", "Добавление расписания", MessageBoxButtons.OK, MessageBoxIcon.Error);
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
            Form view = new ViewForm
            {
                Left = this.Left,
                Top = this.Top
            };
            view.Show();
            this.Hide();
            //    }
            //   else
            //       MessageBox.Show("Arduino не подключен", "Полное расписание", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }



        private void DisconnectButton_Click(object sender, EventArgs e)
        {
            ConSerialPort.Disconnect();
            if (!ConSerialPort.connected)
            {
                connectButton.Enabled = true;
                disconnectButton.Enabled = false;
            }
            else
            {
                MessageBox.Show("Возникла ошибка при отключении", "Отключение Arduino", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
