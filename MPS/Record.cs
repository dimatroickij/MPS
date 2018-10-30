using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MPS
{
    /// <summary>
    /// Класс - запись в памяти EPROM Arduino
    /// </summary>
    class Record
    {
        /// <summary>
        /// Время начала / конца работы расписания
        /// </summary>
        private DateTime startTime, endTime;
        /// <summary>
        /// Номер реле
        /// </summary>
        private int numberDevice;
        /// <summary>
        /// Рабочие дни
        /// </summary>
        private int week = 0;
        /// <summary>
        /// Номер записи
        /// </summary>
        private int numTimer;

        /// <summary>
        /// Конструктор класса
        /// </summary>
        /// <param name="startTime">Начальное время</param>
        /// <param name="endTime">Конечное время</param>
        /// <param name="numberDevice">Номер реле</param>
        /// <param name="week">дни недели</param>
        public Record(DateTime startTime, DateTime endTime, String numberDevice, int week, int numTimer)
        {
            this.StartTime = startTime.AddSeconds(-startTime.Second);
           // this.StartTime.AddSeconds(-startTime.Second);

            this.EndTime = endTime.AddSeconds(-endTime.Second);
            //this.EndTime.AddSeconds(-endTime.Second);

            this.NumberDevice = int.Parse(numberDevice);
            this.Week = week;
            this.NumTimer = numTimer;
        }

        /// <summary>
        /// Конструктор класса
        /// </summary>
        /// <param name="str">Строка</param>
        /// <param name="num">Номер записи</param>
        public Record(string[] str, int num)
        {
            this.StartTime = DateTime.Parse(str[0] + ":" + str[1]);
            this.EndTime = DateTime.Parse(str[2] + ":" + str[3]);
            this.NumberDevice = int.Parse(str[4]);
            this.Week = int.Parse(str[5]);
            this.NumTimer = num;
        }

        /// <summary>
        /// Конструктор класса
        /// </summary>
        /// <param name="row">Строка</param>
        public Record(DataGridViewRow row)
        {
            this.startTime = ((DateTime)row.Cells[0].Value).AddSeconds(-startTime.Second);
            //this.startTime.AddSeconds(-startTime.Second);

            this.endTime = ((DateTime)row.Cells[1].Value).AddSeconds(-endTime.Second);
            // this.endTime.AddSeconds(-endTime.Second);

            this.numberDevice = int.Parse(row.Cells[2].Value.ToString());
            this.week = GetWeek(row);
            this.NumTimer = row.Index;
        }

        /// <summary>
        /// get / set поля startTime
        /// </summary>
        public DateTime StartTime { get => startTime; set => startTime = value; }

        /// <summary>
        /// get / set поля startTime типа String
        /// </summary>
        public String StartTimeStr
        {
            get => startTime.ToString("t").Length != 5 ? "0" + startTime.ToString("t") : startTime.ToString("t");
        }

        /// <summary>
        /// get / set поля endTime типа String
        /// </summary>
        public String EndTimeStr
        {
            get => endTime.ToString("t").Length != 5 ? "0" + endTime.ToString("t") : endTime.ToString("t");
        }

        /// <summary>
        /// get / set поля startTime типа Unix
        /// </summary>
        public int StartTimeUnix
        {
            get => getUnixTime(startTime);
        }

        /// <summary>
        /// get / set поля endTime типа Unix
        /// </summary>
        public int EndTimeUnix
        {
            get => getUnixTime(endTime);
        }

        /// <summary>
        /// get / set поля endTime
        /// </summary>
        public DateTime EndTime { get => endTime; set => endTime = value; }
        /// <summary>
        /// get / set поля numberDevice
        /// </summary>
        public int NumberDevice { get => numberDevice; set => numberDevice = value; }
        /// <summary>
        /// get / set поля week
        /// </summary>
        public int Week { get => week; set => week = value; }

        /// <summary>
        /// Получение рабочих дней в массиве
        /// </summary>
        public String getArrayWeek()
        {
            return Convert.ToString(this.week, 2);
        }

        /// <summary>
        /// get / set поля numTimer
        /// </summary>
        public int NumTimer { get => numTimer; set => numTimer = value; }

        /// <summary>
        /// Получение строки из Unix
        /// </summary>
        public String getString()
        {
            return numTimer.ToString() + "," + StartTimeStr + "," + EndTimeStr + "," + numberDevice.ToString() + "," + week.ToString();
        }

        /// <summary>
        /// Получение Unix из DateTime
        /// </summary>
        /// <param name="d">Дата</param>
        private int getUnixTime(DateTime d)
        {
            int time = (int)(d - new DateTime(1970, 1, 1)).TotalSeconds;
            return time;
        }

        /// <summary>
        /// Получение week из 7 combobox-ов
        /// </summary>
        /// <param name="row">Строка datagridView</param>
        /// <returns></returns>
        private int GetWeek(DataGridViewRow row)
        {

            int w = 0;
            for (int i = 3; i < row.Cells.Count - 2; i++)
            {
                try
                {
                    if ((bool)row.Cells[i].Value)
                        w += (int)Math.Pow(2, 9 - i);
                }
                catch
                {
                    if (row.Cells[i].Value.ToString() == "")
                        w += (int)Math.Pow(2, 9 - i);
                }
            }
            return w;
        }
    }
}
