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

        public bool SaveRecord()
        {
            if (ConSerialPort.connected)
                return ConSerialPort.save(this.startTime.ToString("t") + "," + this.endTime.ToString("t") + "," + (this.numberDevice + 1).ToString() + "," + this.week.ToString());
            else
                return false;
        }
    }
}
