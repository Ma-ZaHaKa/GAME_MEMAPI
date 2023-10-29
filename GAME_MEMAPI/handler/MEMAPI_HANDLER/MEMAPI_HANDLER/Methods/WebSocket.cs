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
        public static WebSocket socket = null;
        public static bool WebSocketReady = false;
        public static void Disable(Logger logger)
        {
            var field = logger.GetType().GetField("_output", BindingFlags.NonPublic | BindingFlags.Instance);
            field?.SetValue(logger, new Action<LogData, string>((d, s) => { }));
        }

        public static async void UWSConectAsync(int port)
        {
            await Task.Run(() =>
            {
                while (true)
                {
                    socket = new WebSocket($"ws://localhost:{port}");
                    Disable(socket.Log);
                    //socket.OnClose += (sender, e) => { };
                    //socket.OnError += (sender, e) => { };

                    //socket.OnMessage += OnMsg;
                    try
                    {
                        socket.Connect();
                        if (socket.ReadyState == WebSocketState.Open)
                        {
                            Console.WriteLine($"Connected!");
                            WebSocketReady = true;
                            break;
                        }
                    }
                    catch { /*Console.Clear();*/ }
                }
            });
        }

        public static bool UWSConect(int port)
        {
            try
            {
                socket = new WebSocket($"ws://localhost:{port}");
                Disable(socket.Log);
                //socket.OnClose += (sender, e) => { };
                //socket.OnError += (sender, e) => { };

                //socket.OnMessage += OnMsg;
                try
                {
                    socket.Connect();
                    if (socket.ReadyState == WebSocketState.Open)
                    {
                        Console.WriteLine($"Connected!");
                        return true;
                    }
                }
                catch { /*Console.Clear();*/ }
                return false;
            }
            catch { return false; }
        }
        public static bool UWSSend(string message)
        {
            try
            {
                if ((socket!= null) && (socket.ReadyState == WebSocketState.Open)) { socket.Send(message); }
                return true;
            }
            catch { return false; }
        }

        public static string GetMessage()
        {
            string last_msg = "";
            socket.OnMessage += OnMsg;
            bool on_message_flag = false;
            while (true)
            {
                if (on_message_flag) { return last_msg; }
                System.Threading.Thread.Sleep(10);
            }

            void OnMsg(object sender, MessageEventArgs e)
            {
                last_msg = e.Data;
                on_message_flag = true;
            }
        }

    }
}
