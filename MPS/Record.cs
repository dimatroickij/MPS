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
        int numTimer;

        /// <summary>
        /// Конструктор класса
        /// </summary>
        /// <param name="startTime">Начальное время</param>
        /// <param name="endTime">Конечное время</param>
        /// <param name="numberDevice">Номер реле</param>
        /// <param name="week">дни недели</param>
        public Record(DateTime startTime, DateTime endTime, String numberDevice, int week, int numTimer)
        {
            this.StartTime = startTime;
            this.EndTime = endTime;
            this.NumberDevice = int.Parse(numberDevice);
            this.Week = week;
            this.NumTimer = numTimer;
        }

        public DateTime StartTime { get => startTime; set => startTime = value; }
        public DateTime EndTime { get => endTime; set => endTime = value; }
        public int NumberDevice { get => numberDevice; set => numberDevice = value; }
        public int Week { get => week; set => week = value; }
        public int NumTimer { get => numTimer; set => numTimer = value; }
    }
}
