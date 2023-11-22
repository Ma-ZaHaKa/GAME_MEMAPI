using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.IO.Ports;
using System.Reflection;
using System.Threading.Tasks;
using WebSocketSharp;
using System.Threading;

namespace MEMAPI_HANDLER
{
    partial class Program
    {
        public static WebSocket socket = null;
        public static bool WebSocketReady = false;
        public static int port = 9005;
        //public static bool close_flag = false;
        public static void WSLogDisable(Logger logger)
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
                    WSLogDisable(socket.Log);
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

        public static bool UWSConect(int _port)
        {
            try
            {
                port = _port;
                socket = new WebSocket($"ws://localhost:{_port}");
                WSLogDisable(socket.Log);
                //socket.OnClose += (sender, e) => { };
                //socket.OnError += (sender, e) => { };

                //socket.OnMessage += OnMsg;
                try
                {
                    socket.Connect();
                    if (socket.ReadyState == WebSocketState.Open)
                    {
                        //close_flag = true;
                        Console.WriteLine($"Connected!");
                        return true;
                    }
                }
                catch { /*Console.Clear();*/ }
                return false;
            }
            catch { return false; }
        }

        public static (bool, string) UWSSend(string message)
        {
            int delay_while = 10; // ms
            List<string> messages = new List<string>();
            void Socket_OnMessage(object sender, MessageEventArgs e) { messages.Add(e.Data); }
            //if ((socket == null) || (socket?.ReadyState != WebSocketState.Open)) { while (!UWSConect(port)) { Thread.Sleep(delay_while); } }
            if ((socket == null) || (socket?.ReadyState != WebSocketState.Open)) { if (!UWSConect(port)) { return (false, ""); } }
            socket.OnMessage += new EventHandler<MessageEventArgs>(Socket_OnMessage);
            socket.Send(message);
            while (true)
            {
                if (socket.ReadyState != WebSocketState.Open) { return (false, ""); }
                else if ((messages.Count == 0)) { Thread.Sleep(delay_while); }
                else { return (true, messages.FirstOrDefault()); } // null default
            }
            return (false, "");
        }



        public static bool UWSSendMsg(string message)
        {
            try { if ((socket != null) && (socket.ReadyState == WebSocketState.Open)) { socket.Send(message); return true; } } catch { }
            return false;
        }
        public static bool UWSGetState()
        {
            if ((socket != null) && (socket.ReadyState == WebSocketState.Open)) { return true; }
            return false;
        }





        public static string CustomGetMessage()
        {
            string last_msg = "";
            socket.OnMessage += OnMsg;
            bool on_message_flag = false;
            while (true)
            {
                if (on_message_flag) { socket.OnMessage -= OnMsg; return last_msg; }
                System.Threading.Thread.Sleep(10);
            }

            void OnMsg(object sender, MessageEventArgs e)
            {
                last_msg = e.Data;
                on_message_flag = true;
            }
        }
        public static string GetMessage()
        {
            string lastMsg = null;
            ManualResetEvent messageReceived = new ManualResetEvent(false);

            socket.OnMessage += Sender;

            void Sender(object sender, MessageEventArgs e)
            {
                lastMsg = e.Data;
                messageReceived.Set();
            };

            while (true)
            {
                messageReceived.WaitOne();
                messageReceived.Reset();
            }

            socket.OnMessage -= Sender;
            return lastMsg;
        }
    }
}
