using System;
using System.Net;
using System.Net.Sockets;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using WebSocketSharp;
using WebSocketSharp.Server;

namespace UWS_DK_Sender
{
    class Program
    {
        static WebSocket socket = null;
        static bool WebSocketReady = false;
        public static void Disable(Logger logger)
        {
            var field = logger.GetType().GetField("_output", BindingFlags.NonPublic | BindingFlags.Instance);
            field?.SetValue(logger, new Action<LogData, string>((d, s) => { }));
        }

        static async void UWSConect()
        {
            await Task.Run(() =>
            {
                while (true)
                {
                    socket = new WebSocket("ws://localhost:9005");
                    Disable(socket.Log);
                    //socket.OnClose += (sender, e) => { };
                    //socket.OnError += (sender, e) => { };

                    socket.OnMessage += OnMsg;
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



        static void OnMsg(object sender, MessageEventArgs e)
        {
            //if(e.Data == "Hello!")
            Console.WriteLine($"Received message: {e.Data}");
            //socket.Send(text);
        }


        static void Main(string[] args)
        {
            Console.ReadKey();
            Console.WriteLine($"Starting UWS!");
            UWSConect();

            string _msg = "";
            while (true)
            {
                _msg = Console.ReadLine().Trim();
                socket.Send(_msg);
            }
        }
    }
}
