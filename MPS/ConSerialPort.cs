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
                MessageBox.Show("Не включён режим СОЕДИНЕНИЕ С ПК", "Подключение", MessageBoxButtons.OK, MessageBoxIcon.Error);
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
            if (connected)
            {
                try
                {
                    sp.Write(str);
                    string returnMessage = sp.ReadLine();
                    if (returnMessage.Contains("y"))
                        return true;
                    else
                        return false;
                }
                catch
                {
                    MessageBox.Show("На устройстве не включён режим ПОЛУЧЕНИЕ ДАННЫХ", "Сохранение данных", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
            }
            else
            {
                MessageBox.Show("Устройство не подключено", "Сохранение данных", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
        }

        /// <summary>
        /// Чтение данных
        /// </summary>
        /// <returns></returns>
        public static String[] Read()
        {
            String[] er = new string[1];
            if (connected)
            {
                int count = 0;
                try
                {
                    string returnMessage = sp.ReadLine();
                    if (returnMessage.Contains("count"))
                    {
                        count = int.Parse(returnMessage.Split('-')[1]);
                        String[] str = new String[count];// = new String[count];
                        sp.Write("r");
                        for (int i = 0; i < count; i++)
                        {
                            if (sp.ReadLine().Contains("r" + i.ToString()))
                            {
                                str[i] = sp.ReadLine().Split('r')[1];
                                if (i != count - 1)
                                    sp.Write("n");
                                else
                                    sp.Write("e");
                            }
                        }
                        return str;
                    }
                    else
                        return er;
                }
                catch
                {
                    MessageBox.Show("Устройство не переведено в меню ПОЛУЧЕНИЕ ДАННЫХ", "Получение данных", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return er;
                }
            }
            else
            {
                MessageBox.Show("Устройство не подключено", "Получение данных", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return er;
            }
        }
    }
}
