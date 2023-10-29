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
        static string FindCOMPort(string my_msg, string response_msg)
        {
            string[] portNames = SerialPort.GetPortNames();

            foreach (string portName in portNames)
            {
                using (SerialPort port = new SerialPort(portName))
                {
                    try
                    {
                        port.Open();
                        port.Write(my_msg);
                        System.Threading.Thread.Sleep(100);
                        if (port.ReadExisting().Contains(response_msg)) { return portName; }
                    }
                    //catch (Exception ex) { Console.WriteLine($"Error communicating with {portName}: {ex.Message}"); }
                    catch { }
                }
            }
            return "";
        }


    }
}
