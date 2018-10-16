﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MPS
{
    public partial class AddForm : Form
    {
        public AddForm()
        {
            InitializeComponent();
            RelayListBox.SetSelected(0, true);
            EndTimePicker.Value = DateTime.Now.AddMinutes(1);

        }

        /// <summary>
        /// Закрытие формы добавления расписания, открытие главной формы
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void AddForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            Form main = Application.OpenForms[0];
            main.StartPosition = FormStartPosition.Manual;
            main.Left = this.Left;
            main.Top = this.Top;
            main.Show();
        }

        /// <summary>
        /// Закрытие формы добавления расписания, открытие главной формы
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void BackButton_Click(object sender, EventArgs e)
        {
            Form main = Application.OpenForms[0];
            main.StartPosition = FormStartPosition.Manual;
            main.Left = this.Left;
            main.Top = this.Top;
            main.Show();
            this.Close();
        }

        /// <summary>
        /// Сохранение данных в память Arduino, переход на главную форму
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SaveButton_Click(object sender, EventArgs e)
        {

            bool[] week = new bool[7] { false, false, false, false, false, false, false };
            if ((WeekCheckedListBox.CheckedItems.Count != 0) && ((EndTimePicker.Value.Subtract(StartTimePicker.Value)).Ticks > 0))
            {
                for (int i = 0; i < WeekCheckedListBox.Items.Count; i++)
                {
                    if (WeekCheckedListBox.GetItemChecked(i))
                        week[i] = true;
                }
            }
            Record rec = new Record(StartTimePicker.Value, EndTimePicker.Value, RelayListBox.SelectedIndex, week);
            if (rec.SaveRecord())
            {
                if (MessageBox.Show("Данные успешно сохранены.", "Сохранение расписания", MessageBoxButtons.OK, MessageBoxIcon.Information) ==
                          System.Windows.Forms.DialogResult.OK)
                    BackButton_Click(sender, e);
                else
                    MessageBox.Show("Возникла ошибка", "Сохранение расписания", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}