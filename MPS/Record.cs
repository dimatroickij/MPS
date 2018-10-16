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
        bool[] week = new bool[7];

        public Record(DateTime startTime, DateTime endTime, int numberDevice, bool[] week)
        {
            this.StartTime = startTime;
            this.EndTime = endTime;
            this.NumberDevice = numberDevice;
            this.Week = week;
        }

        public DateTime StartTime { get => startTime; set => startTime = value; }
        public DateTime EndTime { get => endTime; set => endTime = value; }
        public int NumberDevice { get => numberDevice; set => numberDevice = value; }
        public bool[] Week { get => week; set => week = value; }

        public bool SaveRecord()
        {
            if (ConSerialPort.connected)
            {
                String str = "0";

                foreach (bool val in this.week)
                {
                    str += val ? "1" : "0";
                }

                //MessageBox.Show(this.startTime.ToString("t") + "," + this.endTime.ToString("t") + "," + this.numberDevice.ToString() + "," + str, "Сохранение расписания", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return ConSerialPort.save(this.startTime.ToString("t") + "," + this.endTime.ToString("t") + "," + this.numberDevice.ToString() + "," + str + "#");
            }
            else
                return false;
        }
    }
}
