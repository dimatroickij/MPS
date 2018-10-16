using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Windows.Forms;

namespace MPS
{
    static class ConSerialPort
    {
        static private SerialPort sp = new SerialPort("COM7", 9600);
        static public bool connected = false;


        public static void connect()
        {
            try
            {
                if (SerialPort.GetPortNames().Length == 0)
                {
                    MessageBox.Show("Нет подключённых устройств", "Сохранение расписания", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                else
                {
                    if (!sp.IsOpen)
                        sp.Open();
                    System.Threading.Thread.Sleep(1000); // just wait a lot
                    string returnMessage = sp.ReadLine();
                    if (returnMessage.Contains("Arduino"))
                    {
                        sp.Write("y");
                        connected = true;
                        sp.DiscardInBuffer();
                        sp.DiscardOutBuffer();
                    }
                    else
                    {
                        if (sp.IsOpen)
                            sp.Close();
                        connected = false;
                    }
                }
            }
            catch
            {
                MessageBox.Show("Возникла ошибка", "Сохранение расписания", MessageBoxButtons.OK, MessageBoxIcon.Error);
                if (sp.IsOpen)
                    sp.Close();
                connected = false;

            }
        }

        public static void disconnect()
        {
            if (sp.IsOpen)
            {
                sp.Write("e");
                sp.DiscardInBuffer();
                sp.DiscardOutBuffer();
                sp.Close();
            }
            connected = false;
        }

        public static bool save(String str)
        {
            if (connected)
            {
                sp.Write("s");
                string returnMessage = sp.ReadLine();
                if (returnMessage.Contains("y"))
                    return true;
                else
                    return false;
            }
            else
                return false;
        }
    }
}
