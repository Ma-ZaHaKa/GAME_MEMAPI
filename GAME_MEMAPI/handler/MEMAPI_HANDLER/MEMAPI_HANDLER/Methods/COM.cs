using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.IO.Ports;
using System.Reflection;
using System.Threading.Tasks;
using WebSocketSharp;

namespace MEMAPI_HANDLER
{
    partial class Program
    {
        public static string FindCOMPort(string my_msg, string response_msg)
        {
            string[] portNames = SerialPort.GetPortNames();

            foreach (string portName in portNames)
            {
                SendMsgCom(portName, my_msg);
                if (SendMsgCom(portName, my_msg).Contains(response_msg)) { return portName; }

                //using (SerialPort port = new SerialPort(portName))
                //{
                //    try
                //    {
                //        port.Open();
                //        port.Write(my_msg);
                //        System.Threading.Thread.Sleep(100);
                //        if (port.ReadExisting().Contains(response_msg)) { return portName; }
                //    }
                //    //catch (Exception ex) { Console.WriteLine($"Error communicating with {portName}: {ex.Message}"); }
                //    catch { }
                //}
            }
            return "";
        }
        public static string SendMsgCom(string port_name, string my_msg, int baudRate = 9600)
        {
            SerialPort serialPort = new SerialPort(port_name, baudRate);

            try
            {
                serialPort.Open();
                serialPort.Write(my_msg);
                System.Threading.Thread.Sleep(100);

                // Ждем ответ (можно установить таймаут)
                string response = serialPort.ReadLine();

                return response;
            }
            catch (Exception ex)
            {
                Console.WriteLine("Ошибка: " + ex.Message);
                return string.Empty;
            }
            finally
            {
                serialPort.Close();
            }
        }


    }
}
