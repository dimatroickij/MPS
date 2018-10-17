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
        DateTime startTime, endTime;
        int numberDevice;
        int week = 0;

        /// <summary>
        /// Конструктор класса
        /// </summary>
        /// <param name="startTime">Начальное время</param>
        /// <param name="endTime">Конечное время</param>
        /// <param name="numberDevice">Номер реле</param>
        /// <param name="week">дни недели</param>
        public Record(DateTime startTime, DateTime endTime, int numberDevice, int week)
        {
            this.StartTime = startTime;
            this.EndTime = endTime;
            this.NumberDevice = numberDevice;
            this.Week = week;
        }

        public DateTime StartTime { get => startTime; set => startTime = value; }
        public DateTime EndTime { get => endTime; set => endTime = value; }
        public int NumberDevice { get => numberDevice; set => numberDevice = value; }
        public int Week { get => week; set => week = value; }

        /// <summary>
        /// Сохранение записи через класс порта
        /// </summary>
        /// <returns></returns>
        public bool SaveRecord()
        {
            if (ConSerialPort.connected)
                return ConSerialPort.Save(this.startTime.ToString("t") + "," + this.endTime.ToString("t") + "," + (this.numberDevice + 1).ToString() + "," + this.week.ToString());
            else
            {
                MessageBox.Show("Устройство не подключено", "Сохранение данных", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
        }

    }
}
