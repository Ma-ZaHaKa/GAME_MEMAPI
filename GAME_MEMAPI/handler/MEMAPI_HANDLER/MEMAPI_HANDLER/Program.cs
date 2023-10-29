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
        //public static string last_msg = "";
        //static void OnMsg_(object sender, MessageEventArgs e)
        //{
        //    //if(e.Data == "Hello!")
        //    //Console.WriteLine($"Received message: {e.Data}");
        //    //socket.Send(text);
        //    last_msg = e.Data;
        //}






        static string GMA_START_MSG = "HI";
        static string GMA_DEV_RESPONSE_MSG = "HI_GMA";

        static void Main(string[] args)
        {
            //string foundPort = FindCOMPort(GMA_START_MSG, GMA_DEV_RESPONSE_MSG);
            //if (foundPort == "") { Console.WriteLine($"No port with '{GMA_DEV_RESPONSE_MSG}' response found."); return; }

            int GMA_PORT = 9005;
            bool status_connect = UWSConect(GMA_PORT);
            if (!status_connect) { Console.WriteLine($"WS CANT CONNECT TO PORT {GMA_PORT}"); return; }
            //string json = GetPointerJSON("gta3.exe", "0x5412F0", new List<string> { "0x2C0" });
            string json = GetPointerJSON("gta3.exe", "0x5412F0", new List<string> { "0x2C0" });
            socket.Send(json);
            Console.WriteLine($"{GetMessage()}");

        }
    }
}
