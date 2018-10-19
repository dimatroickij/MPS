using System;
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
    public partial class ViewForm : Form
    {
        public ViewForm()
        {
            InitializeComponent();
        }

        private void ViewForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            Form main = Application.OpenForms[0];
            main.StartPosition = FormStartPosition.Manual;
            main.Left = this.Left;
            main.Top = this.Top;
            main.Show();
        }

        private void ViewForm_Load(object sender, EventArgs e)
        {
            String[] str = ConSerialPort.Read().Split(';');

            for (int i = 0; i < str.Length - 1; i++)
            {
                str[i] = str[i].Trim();
                String[] record = str[i].Split(',');
                dataGridView.Rows.Add();
                for (int j = 0; j < 4; j++)
                {
                    if (record[j].Length != 2)
                        record[j] = "0" + record[j];
                }
                var week = Convert.ToString(int.Parse(record[5]), 2).ToArray();
                dataGridView.Rows[i].Cells[0].Value = record[0] + ":" + record[1];
                dataGridView.Rows[i].Cells[1].Value = record[2] + ":" + record[3];
                dataGridView.Rows[i].Cells[2].Value = record[4];
                int z = 0;
                for (int j = 3; j < dataGridView.Rows[i].Cells.Count - 2; j++)
                {
                    if (j >= dataGridView.Rows[i].Cells.Count - 2 - week.Length)
                    {

                        dataGridView.Rows[i].Cells[j].Value = week[z] == '1' ? true : false;
                        z++;
                    }
                    else
                        dataGridView.Rows[i].Cells[j].Value = false;
                    
                }
                dataGridView.Rows[i].Cells[10].Value = "Изменить";
                dataGridView.Rows[i].Cells[11].Value = "Удалить";
            }
            dataGridView.ColumnHeadersDefaultCellStyle.Alignment = dataGridView.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
        }

        private void dataGridView_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            switch (e.ColumnIndex)
            {

                case 10:
                    String str = "";
                    DataGridViewRow row = dataGridView.Rows[e.RowIndex];
                    //row.ReadOnly = false;
                    str += e.RowIndex.ToString();
                    for (int i = 0; i < row.Cells.Count - 9; i++)
                    {
                        str += "," + row.Cells[i].Value;
                    }

                    int week = 0;

                    for (int i = 3; i < row.Cells.Count - 2; i++)
                    {
                        if ((bool)row.Cells[i].Value)
                            week += (int)Math.Pow(2, 9 - i);
                        //str += "," + row.Cells[i].Value;
                    }

                    str += "," + week.ToString();
                    
                    //if (true)// (ConSerialPort.Edit(""))
                    //    MessageBox.Show(str, "Полное расписание", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    //else
                    //    MessageBox.Show("Возникла ошибка при изменении", "Полное расписание", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    break;

                case 11:
                    if (ConSerialPort.Delete(e.RowIndex.ToString()))
                    {
                        dataGridView.Rows.RemoveAt(e.RowIndex);
                        MessageBox.Show("Расписание удалено", "Полное расписание", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                        MessageBox.Show("Возникла ошибка при удалении", "Полное расписание", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    break;

                default:
                    break;
            }
        }
    }
}
