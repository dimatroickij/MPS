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
        /// <summary>
        /// Порт
        /// </summary>
        static private SerialPort sp = new SerialPort("COM5", 9600);
        /// <summary>
        /// Флаг, показывающий состояние подключения Ардуино
        /// </summary>
        static public bool connected = false;
        /// <summary>
        /// Количество расписаний на данный момент
        /// </summary>
        static public int saveTimer = 0;
        /// <summary>
        /// Максимальное количество расписаний
        /// </summary>
        static public int maxTimers = 0;

        /// <summary>
        /// Конструктор - установка timeout для получения данных от ардуино
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
                    //System.Threading.Thread.Sleep(1000); // just wait a lot
                    sp.Write("c");
                    string returnMessage = sp.ReadLine();
                    if (returnMessage.Contains("Arduino"))
                    {
                        sp.Write("y");
                        String[] timers = sp.ReadLine().Split(',');
                        saveTimer = int.Parse(timers[0]);
                        maxTimers = int.Parse(timers[1]);
                        connected = true;
                        sp.DiscardInBuffer();
                        sp.DiscardOutBuffer();
                    }
                    else
                    {
                        sp.DiscardInBuffer();
                        sp.DiscardOutBuffer();
                        if (sp.IsOpen)
                            sp.Close();
                        connected = false;
                    }
                }
            }
            catch
            {
                MessageBox.Show("Переведите Ардуино в главное меню!", "Подключение", MessageBoxButtons.OK, MessageBoxIcon.Error);
                sp.DiscardInBuffer();
                sp.DiscardOutBuffer();
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
                sp.Write("t");
                sp.DiscardInBuffer();
                sp.DiscardOutBuffer();
                sp.Close();
            }
            saveTimer = maxTimers = 0;
            connected = false;
        }

        /// <summary>
        /// Добавление / изменение данных в Ардуино
        /// </summary>
        /// <param name="mode">Номер режима</param>
        /// <param name="str">Строка, содержащая данные</param>
        /// <returns></returns>
        public static bool EditData(int mode, String str)
        {
            String[][] modeArray = new String[2][];
            modeArray[0] = new String[3];
            modeArray[1] = new String[3];
            modeArray[0][0] = "s";
            modeArray[0][1] = "endSave";
            modeArray[0][2] = "Сохранение данных";

            modeArray[1][0] = "e";
            modeArray[1][1] = "endEdit";
            modeArray[1][2] = "Изменение данных";

            try
            {
                sp.Write(modeArray[mode][0]);
                sp.Write(str);
                String returnMessage = sp.ReadLine();

                if (returnMessage.Contains(modeArray[mode][1]))
                {
                    sp.DiscardInBuffer();
                    sp.DiscardOutBuffer();

                    if (mode == 0)
                        saveTimer++;

                    return true;
                }
                else
                {
                    sp.DiscardInBuffer();
                    sp.DiscardOutBuffer();
                    return false;
                }
            }
            catch
            {
                MessageBox.Show("Устройство не найдено", modeArray[mode][2], MessageBoxButtons.OK, MessageBoxIcon.Error);
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

        /// <summary>
        /// Удаление данных по запросу с компьютера
        /// </summary>
        /// <param name="str">Номер записи</param>
        public static bool Delete(String str)
        {
            try
            {
                sp.Write("d");
                sp.Write(str);
                if (sp.ReadLine().Contains("endDelete"))
                {
                    saveTimer--;
                    return true;
                }
                else
                    return false;
            }
            catch
            {
                return false;
            }
        }

        public static bool DeleteAll()
        {
            try
            {
                sp.Write("h");
                if (sp.ReadLine().Contains("endDeleteAll"))
                {
                    saveTimer = 0;
                    return true;
                }
                else
                    return false;
            }
            catch
            {
                return false;
            }
        }
    }
}
