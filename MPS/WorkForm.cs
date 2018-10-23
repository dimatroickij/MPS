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
    /// <summary>
    /// Класс формы
    /// </summary>
    public partial class WorkForm : Form
    {
        List<Record> records = new List<Record>();

        /// <summary>
        /// Конструктор класса
        /// </summary>
        public WorkForm()
        {
            InitializeComponent();
            String[] str = ConSerialPort.Read().Split(';');

            for (int i = 0; i < str.Length - 1; i++)
            {

                dataGridView.Rows.Add();
                records.Add(new Record(str[i].Trim().Split(','), i));
                dataGridView.Rows[i].Cells[0].Value = records[i].StartTime;
                dataGridView.Rows[i].Cells[1].Value = records[i].EndTime;
                dataGridView.Rows[i].Cells[2].Value = records[i].NumberDevice.ToString();
                SetWeek(dataGridView.Rows[i], records[i].Week);
                dataGridView.Rows[i].Cells[10].Value = "Изменить";
                dataGridView.Rows[i].Cells[11].Value = "Удалить";
                dataGridView.Rows[i].ReadOnly = true;
            }
            dataGridView.ColumnHeadersDefaultCellStyle.Alignment = dataGridView.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

            if (ConSerialPort.saveTimer > ConSerialPort.maxTimers)
                AddButton.Enabled = false;
            if (dataGridView.Rows.Count == 0)
                deleteButton.Enabled = false;
        }

        /// <summary>
        /// Обработка закрытия формы
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ViewForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (ConSerialPort.connected)
                ConSerialPort.Disconnect();
            Form main = Application.OpenForms[0];
            main.Close();

        }

        /// <summary>
        /// Установка combobox-ов из int переменной
        /// </summary>
        /// <param name="row">Строка</param>
        /// <param name="w">число, из которого идёт заполнение</param>
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
        /// <summary>
        /// Обработка клика по ячейкам таблицы
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void dataGridView_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            try
            {
                var row = dataGridView.Rows[e.RowIndex];
                var record = records.Find(r => r.NumTimer == e.RowIndex);
                //  Выбор действия
                switch (e.ColumnIndex)
                {
                    //  Кнопка "Изменить / сохранить"
                    case 10:
                        Record saveRecord;
                        switch (row.Cells[10].Value)
                        {
                            case "Изменить":
                                row.Cells[10].Value = "Сохранить";
                                row.Cells[11].Value = "Отмена";
                                row.ReadOnly = false;
                                break;

                            case "Добавить":
                                saveRecord = new Record(row);
                                if ((saveRecord.EndTime.Subtract(saveRecord.StartTime).Ticks > 0) && (saveRecord.Week > 0))
                                {
                                    List<Record> mas1 = records.FindAll(rec =>
                                        (saveRecord.NumberDevice == rec.NumberDevice) &&
                                        ((saveRecord.Week & rec.Week) != 0) &&
                                        (saveRecord.NumTimer != rec.NumTimer)
                                        );

                                    List<Record> mas2 =
                                        mas1.FindAll(rec =>
                                        ((saveRecord.StartTimeUnix < rec.EndTimeUnix) && (saveRecord.StartTimeUnix >= rec.StartTimeUnix)) ||
                                        ((saveRecord.EndTimeUnix > rec.StartTimeUnix) && (saveRecord.EndTimeUnix <= rec.EndTimeUnix)) ||
                                        ((saveRecord.StartTimeUnix < rec.StartTimeUnix) && (saveRecord.EndTimeUnix > rec.EndTimeUnix))
                                        );
                                    if (mas2.Count == 0)
                                    {
                                        if (ConSerialPort.Save(saveRecord.getString()))
                                        {
                                            MessageBox.Show("Расписание сохранено", "Добавление расписания", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                            records.Add(saveRecord);
                                            row.Cells[10].Value = "Изменить";
                                            row.Cells[11].Value = "Удалить";
                                            row.ReadOnly = true;
                                            Thread.Sleep(1000);
                                            if (ConSerialPort.saveTimer > ConSerialPort.maxTimers)
                                                AddButton.Enabled = false;
                                            if (dataGridView.Rows.Count != 0)
                                                deleteButton.Enabled = true;
                                        }
                                        else
                                            MessageBox.Show("Возникла ошибка при добавлении", "Добавление расписания", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                    }
                                    else
                                        MessageBox.Show("Расписание дублирует уже созданное", "Добавление расписания", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                }
                                else
                                    MessageBox.Show("Введены неправильные данные", "Добавление расписания", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                break;

                            case "Сохранить":
                                saveRecord = new Record(row);
                                if ((saveRecord.EndTime.Subtract(saveRecord.StartTime).Ticks > 0) && (saveRecord.Week > 0))
                                {
                                    List<Record> mas1 = records.FindAll(rec =>
                                        (saveRecord.NumberDevice == rec.NumberDevice) &&
                                        ((saveRecord.Week & rec.Week) != 0) &&
                                        (saveRecord.NumTimer != rec.NumTimer)
                                        );

                                    List<Record> mas2 =
                                        mas1.FindAll(rec =>
                                       ((saveRecord.StartTimeUnix < rec.EndTimeUnix) && (saveRecord.StartTimeUnix >= rec.StartTimeUnix)) ||
                                        ((saveRecord.EndTimeUnix > rec.StartTimeUnix) && (saveRecord.EndTimeUnix <= rec.EndTimeUnix)) ||
                                        ((saveRecord.StartTimeUnix < rec.StartTimeUnix) && (saveRecord.EndTimeUnix > rec.EndTimeUnix))
                                        );
                                    if (mas2.Count == 0)
                                    {
                                        if (ConSerialPort.Edit(saveRecord.getString()))
                                        {
                                            MessageBox.Show("Расписание изменено", "Изменение расписания", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                            row.Cells[10].Value = "Изменить";
                                            row.Cells[11].Value = "Удалить";
                                            row.ReadOnly = true;
                                            records.Remove(record);
                                            records.Insert(saveRecord.NumTimer, saveRecord);
                                            Thread.Sleep(1000);
                                        }
                                        else
                                            MessageBox.Show("Возникла ошибка при изменении", "Изменение расписания", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                    }
                                    else
                                        MessageBox.Show("Расписание дублирует уже созданное", "Изменение расписания", MessageBoxButtons.OK, MessageBoxIcon.Error);

                                }
                                else
                                    MessageBox.Show("Введены неправильные данные", "Изменение расписания", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                break;
                        }
                        break;

                    //  Кнопка "Удалить / отмена"
                    case 11:

                        switch (row.Cells[11].Value)
                        {
                            case "Удалить":
                                if (ConSerialPort.Delete(e.RowIndex.ToString()))
                                {
                                    var r = records.FindAll(rr => rr.NumTimer > e.RowIndex);
                                    records.Remove(record);
                                    records.ForEach(rec => rec.NumTimer = records.FindIndex(rr => rr == rec));

                                    dataGridView.Rows.RemoveAt(e.RowIndex);
                                    MessageBox.Show("Расписание удалено", "Полное расписание", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                    Thread.Sleep(1000);
                                    AddButton.Enabled = true;
                                    if (dataGridView.Rows.Count == 0)
                                        deleteButton.Enabled = false;
                                }
                                else
                                    MessageBox.Show("Возникла ошибка при удалении", "Полное расписание", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                break;

                            case "Отмена":
                                row.Cells[10].Value = "Изменить";
                                row.Cells[11].Value = "Удалить";
                                row.ReadOnly = true;

                                row.Cells[0].Value = record.StartTime;
                                row.Cells[1].Value = record.EndTime;
                                row.Cells[2].Value = record.NumberDevice.ToString();
                                SetWeek(row, record.Week);
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
        /// <summary>
        /// Обработка нажатия на кнопку "Добавление расписания"
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void AddButton_Click(object sender, EventArgs e)
        {
            DateTime now = DateTime.Now;
            now = now.AddSeconds(-now.Second);
            var z = dataGridView.Rows.Add(now, now.AddMinutes(1), "1", false, false, false, false, false, false, false, "Добавить", "Отменить");
        }

        private void deleteButton_Click(object sender, EventArgs e)
        {
            foreach (DataGridViewRow row in dataGridView.Rows)
            {
                ConSerialPort.Delete(row.Index.ToString());
                dataGridView.Rows.Remove(row);
                Thread.Sleep(1000);
            }
            records.RemoveAll(rec => rec.NumTimer != 150);
            deleteButton.Enabled = false;
        }
    }
}
