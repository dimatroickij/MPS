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
    public partial class WorkForm : Form
    {
        List<Record> records = new List<Record>();
        public WorkForm()
        {
            InitializeComponent();
        }

        private void ViewForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (ConSerialPort.connected)
                ConSerialPort.Disconnect();
            Form main = Application.OpenForms[0];
            main.Close();

        }

        private void ViewForm_Load(object sender, EventArgs e)
        {
            String[] str = ConSerialPort.Read().Split(';');

            for (int i = 0; i < str.Length - 1; i++)
            {
                dataGridView.Rows.Add();
                str[i] = str[i].Trim();
                String[] record = str[i].Split(',');

                for (int j = 0; j < 4; j++)
                {
                    if (record[j].Length != 2)
                        record[j] = "0" + record[j];
                }

                var week = Convert.ToString(int.Parse(record[5]), 2).ToArray();
                dataGridView.Rows[i].Cells[0].Value = DateTime.Parse(record[0] + ":" + record[1]);
                dataGridView.Rows[i].Cells[1].Value = DateTime.Parse(record[2] + ":" + record[3]);
                dataGridView.Rows[i].Cells[2].Value = record[4];

                SetWeek(dataGridView.Rows[i], int.Parse(record[5]));
                for (int j = 3; j < dataGridView.Rows[i].Cells.Count - 2; j++)
                    dataGridView.Rows[i].Cells[j].Value = false;

                int z = 0;
                for (int j = dataGridView.Rows[i].Cells.Count - 2 - week.Length; j < dataGridView.Rows[i].Cells.Count - 2; j++)
                {

                    dataGridView.Rows[i].Cells[j].Value = week[z] == '1' ? true : false;
                    z++;
                }
                dataGridView.Rows[i].Cells[10].Value = "Изменить";
                dataGridView.Rows[i].Cells[11].Value = "Удалить";
                dataGridView.Rows[i].ReadOnly = true;
            }
            dataGridView.ColumnHeadersDefaultCellStyle.Alignment = dataGridView.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

            if (ConSerialPort.saveTimer > ConSerialPort.maxTimers)
                AddButton.Enabled = false;
        }

        private int GetWeek(DataGridViewRow r)
        {

            int w = 0;
            for (int i = 3; i < r.Cells.Count - 2; i++)
            {
                try
                {
                    if ((bool)r.Cells[i].Value)
                        w += (int)Math.Pow(2, 9 - i);
                }
                catch
                {
                    if (r.Cells[i].Value.ToString() == "")
                        w += (int)Math.Pow(2, 9 - i);
                }
            }
            return w;
        }

        private void SetWeek(DataGridViewRow row, int w)
        {
            var week = Convert.ToString(w, 2).ToArray();
            int z = 0;
            for (int j = 3; j < row.Cells.Count - 2; j++)
                row.Cells[j].Value = false;
            for (int j = row.Cells.Count - 2 - week.Length; j < row.Cells.Count - 2; j++)
            {
                row.Cells[j].Value = week[z] == '1' ? true : false;
                z++;
            }
        }

        private void dataGridView_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            try
            {
                var row = dataGridView.Rows[e.RowIndex];

                switch (e.ColumnIndex)
                {
                    case 10:
                        switch (row.Cells[10].Value)
                        {
                            case "Изменить":
                                row.Cells[10].Value = "Сохранить";
                                row.Cells[11].Value = "Отмена";
                                row.ReadOnly = false;
                                records.Add(new Record((DateTime)row.Cells[0].Value, (DateTime)row.Cells[1].Value, (String)row.Cells[2].Value, GetWeek(row), row.Index));
                                break;

                            case "Добавить":
                                String str = "";

                                DateTime[] datetimes = new DateTime[2];
                                datetimes[0] = (DateTime)row.Cells[0].Value;
                                datetimes[1] = (DateTime)row.Cells[1].Value;
                                if ((datetimes[1].Subtract(datetimes[0]).Ticks > 0) && (GetWeek(row) > 0))
                                {
                                    String startTime = datetimes[0].ToString("t");
                                    String endTime = datetimes[1].ToString("t");
                                    startTime = startTime.Length != 5 ? "0" + startTime : startTime;
                                    endTime = endTime.Length != 5 ? "0" + endTime : endTime;
                                    str += startTime + ";" + endTime;
                                    str += ";" + row.Cells[2].Value + ";" + GetWeek(row).ToString();

                                    if (ConSerialPort.Save(str))
                                    {
                                        MessageBox.Show("Данные сохранены", "Полное расписание", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                        row.Cells[10].Value = "Изменить";
                                        row.Cells[11].Value = "Удалить";
                                        row.ReadOnly = true;
                                        if (ConSerialPort.saveTimer > ConSerialPort.maxTimers)
                                            AddButton.Enabled = false;
                                    }
                                    else
                                        MessageBox.Show("Возникла ошибка при изменении", "Полное расписание", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                }
                                else
                                    MessageBox.Show("Введены неправильные данные", "Полное расписание", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                break;

                            case "Сохранить":
                                String str2 = "";
                                str2 += e.RowIndex.ToString();
                                DateTime[] datetimes2 = new DateTime[2];
                                datetimes2[0] = (DateTime)row.Cells[0].Value;
                                datetimes2[1] = (DateTime)row.Cells[1].Value;
                                if ((datetimes2[1].Subtract(datetimes2[0]).Ticks > 0) && (GetWeek(row) > 0))
                                {
                                    String startTime = datetimes2[0].ToString("t");
                                    String endTime = datetimes2[1].ToString("t");
                                    startTime = startTime.Length != 5 ? "0" + startTime : startTime;
                                    endTime = endTime.Length != 5 ? "0" + endTime : endTime;
                                    str2 += ";" + startTime + ";" + endTime;
                                    str2 += ";" + row.Cells[2].Value + ";" + GetWeek(row).ToString();

                                    if (ConSerialPort.Edit(str2))
                                    {
                                        MessageBox.Show("Данные изменены", "Полное расписание", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                        row.Cells[10].Value = "Изменить";
                                        row.Cells[11].Value = "Удалить";
                                        row.ReadOnly = true;
                                        var record = records.Find(r => r.NumTimer == row.Index);
                                        records.Remove(record);
                                    }
                                    else
                                        MessageBox.Show("Возникла ошибка при изменении", "Полное расписание", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                }
                                else
                                    MessageBox.Show("Введены неправильные данные", "Полное расписание", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                break;
                        }
                        break;

                    case 11:
                        switch (row.Cells[11].Value)
                        {
                            case "Удалить":
                                if (ConSerialPort.Delete(e.RowIndex.ToString()))
                                {
                                    dataGridView.Rows.RemoveAt(e.RowIndex);
                                    MessageBox.Show("Расписание удалено", "Полное расписание", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                    AddButton.Enabled = true;
                                }
                                else
                                    MessageBox.Show("Возникла ошибка при удалении", "Полное расписание", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                break;

                            case "Отмена":
                                row.Cells[10].Value = "Изменить";
                                row.Cells[11].Value = "Удалить";
                                row.ReadOnly = true;

                                var record = records.Find(r => r.NumTimer == row.Index);
                                row.Cells[0].Value = record.StartTime.ToString("t");
                                row.Cells[1].Value = record.EndTime.ToString("t");
                                row.Cells[2].Value = record.NumberDevice.ToString();
                                SetWeek(row, record.Week);
                                records.Remove(record);
                                break;

                            case "Отменить":
                                dataGridView.Rows.RemoveAt(e.RowIndex);
                                break;
                        }
                        break;

                    default:
                        break;
                }
            }
            catch
            {

            }
        }

        private void AddButton_Click(object sender, EventArgs e)
        {
            var z = dataGridView.Rows.Add(DateTime.Now, (DateTime.Now).AddMinutes(1), "1", false, false, false, false, false, false, false, "Добавить", "Отменить");
        }
    }
}
