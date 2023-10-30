﻿using Newtonsoft.Json;
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
        //public static void COMLogDisable(Logger logger)
        //{
        //    var field = logger.GetType().GetField("_output", BindingFlags.NonPublic | BindingFlags.Instance);
        //    field?.SetValue(logger, new Action<LogData, string>((d, s) => { }));
        //}
        static string FixString4Serial(string str) { return str.Replace("\r", "").Replace("\n", "").Replace("\t", ""); }

        public static bool LOG_SEARCH_COM = false;
        //public static int timeout = 1000; // 1sec
        public static int timeout = 1500; // 1sec

        public static string FindCOMPort(string response_msg)
        {
            string[] portNames = SerialPort.GetPortNames();

            foreach (string portName in portNames)
            {
                if (LOG_SEARCH_COM) { Console.WriteLine($"TRYING: {portName}"); }
                string tmp_msg = SendMsgCom(portName, GetHelloJSON());
                if (tmp_msg.Contains(response_msg)) { return portName; }

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
                serialPort.Write(FixString4Serial(my_msg));
                System.Threading.Thread.Sleep(100);

                // Ждем ответ (можно установить таймаут)
                serialPort.ReadTimeout = timeout;
                serialPort.WriteTimeout = timeout;
                string response = serialPort.ReadLine();

                return response;
            }
            catch (Exception ex)
            {
                //Console.WriteLine("Ошибка: " + ex.Message);
                return string.Empty;
            }
            finally
            {
                serialPort.Close();
            }
        }
        public static void SendMsgComNoResp(string port_name, string my_msg, int baudRate = 9600)
        {
            SerialPort serialPort = new SerialPort(port_name, baudRate);
            try
            {
                serialPort.Open();
                serialPort.Write(FixString4Serial(my_msg));
                System.Threading.Thread.Sleep(100);

                // Ждем ответ (можно установить таймаут)
                serialPort.ReadTimeout = timeout;
                serialPort.WriteTimeout = timeout;
            }
            catch (Exception ex) { }
            finally
            {
                serialPort.Close();
            }
        }
        public async static void SendMsgComNoRespAsync(string port_name, string my_msg, int baudRate = 9600)
        {
            //await Task.Run(() => // EXCEPTION NULL REF
            {
                SerialPort serialPort = new SerialPort(port_name, baudRate);
                try
                {
                    serialPort.Open();
                    serialPort.Write(FixString4Serial(my_msg));
                    System.Threading.Thread.Sleep(100);

                    // Ждем ответ (можно установить таймаут)
                    serialPort.ReadTimeout = timeout;
                    serialPort.WriteTimeout = timeout;
                }
                catch (Exception ex) { }
                finally
                {
                    serialPort.Close();
                }
                //});
            }
        }
    }
}
