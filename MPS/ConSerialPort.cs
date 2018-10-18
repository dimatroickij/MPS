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
        static private SerialPort sp = new SerialPort("COM5", 9600);
        static public bool connected = false;

        /// <summary>
        /// Констуктор - установка timeout для получения данных от ардуино
        /// </summary>
        static ConSerialPort()
        {
            sp.ReadTimeout = 4000;
        }

        /// <summary>
        /// Подключение устройства к компьютеру
        /// </summary>
        public static void Connect()
        {
            try
            {
                if (SerialPort.GetPortNames().Length == 0)
                {
                    MessageBox.Show("Нет подключённых устройств", "Подключение", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                else
                {
                    sp.PortName = SerialPort.GetPortNames()[0];
                    if (!sp.IsOpen)
                        sp.Open();
                    System.Threading.Thread.Sleep(1000); // just wait a lot
                    sp.Write("c");
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
                MessageBox.Show("Переведите Ардуино в главное меню!", "Подключение", MessageBoxButtons.OK, MessageBoxIcon.Error);
                if (sp.IsOpen)
                    sp.Close();
                connected = false;
            }
        }

        /// <summary>
        /// Отключение устройства от компьютера
        /// </summary>
        public static void Disconnect()
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

        /// <summary>
        /// Сохранение данных в ардуино
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        public static bool Save(String str)
        {
            try
            {
                sp.Write("s");
                sp.Write(str);
                string returnMessage = sp.ReadLine();

                if (returnMessage.Contains("endSave"))
                    return true;
                else
                    return false;
            }
            catch
            {
                MessageBox.Show("Устройство не найдено", "Сохранение данных", MessageBoxButtons.OK, MessageBoxIcon.Error);
                sp.DiscardInBuffer();
                sp.DiscardOutBuffer();
                return false;
            }
        }


        /// <summary>
        /// Чтение данных
        /// </summary>
        /// <returns></returns>
        public static String Read()
        {
            try
            {
                String str = "";
                sp.Write("r");
                if (sp.ReadLine().Contains("yesRead"))
                {
                    String ss = sp.ReadLine();
                    while (!ss.Contains("endRead"))
                    {
                        str += ss;
                        ss = sp.ReadLine();
                    }
                    sp.DiscardInBuffer();
                    sp.DiscardOutBuffer();
                    return str;
                }
                else
                {
                    sp.DiscardInBuffer();
                    sp.DiscardOutBuffer();
                    return "";
                }
            }
            catch
            {
                MessageBox.Show("Устройство не найдено", "Получение данных", MessageBoxButtons.OK, MessageBoxIcon.Error);
                sp.DiscardInBuffer();
                sp.DiscardOutBuffer();
                return "";
            }
        }
    }
}
