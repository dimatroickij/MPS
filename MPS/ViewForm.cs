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
                for (int j = dataGridView.Rows[i].Cells.Count - 2 - week.Length; j < dataGridView.Rows[i].Cells.Count - 2; j++)
                {

                    dataGridView.Rows[i].Cells[j].Value = week[z] == '1' ? true : false;
                    z++;
                }
            }
            dataGridView.ColumnHeadersDefaultCellStyle.Alignment = dataGridView.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

        }
    }
}
