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
            // { "mode":"get_pointer", "module" : "fmodstudio.dll", "base_offset" : "0x158B50", "offsets" : ["0x5C0", "0x8", "0x60", "0x60", "0x18", "0x208"] }
            //string foundPort = FindCOMPort(GMA_START_MSG, GMA_DEV_RESPONSE_MSG);
            //if (foundPort == "") { Console.WriteLine($"No port with '{GMA_DEV_RESPONSE_MSG}' response found."); return; }
            //195710D2FD0
            int GMA_PORT = 9005;
            int delay = 1 * 1000;
            bool status_connect = false;
            //if (!status_connect) { Console.WriteLine($"WS CANT CONNECT TO PORT {GMA_PORT}"); return; }

            string json = GetPointerJSON("gta3.exe", "0x5412F0", new List<string> { "0x2C0" }); // x86 test
            //json = GetPointerJSON("fmodstudio.dll", "0x158B50", new List<string> { "0x5C0", "0x8", "0x60", "0x60", "0x18", "0x208" }); // x64 test

            Console.WriteLine(json + "\n");
            //json = GetGetValueJSON("0x0", "float");

            string calc_pointer = "";
            while (true)
            {
                if (!UWSGetState()) { UWSConect(GMA_PORT); }
                if (!UWSSend(json)) { Console.WriteLine("WS COUD NOT CONNECT!"); System.Threading.Thread.Sleep(delay); continue; }
                string answ = GetMessage();
                if (answ[0] == '~') { Console.WriteLine("MEM CHAIN POINTERS NOT INIT!"); System.Threading.Thread.Sleep(delay); continue; }
                Console.WriteLine($"{answ}");
                calc_pointer = answ;
                break;
            }

            json = GetGetValueJSON(calc_pointer, "float");
            while (true)
            {
                if ((!UWSSend(json)) || (!UWSGetState())) { Console.WriteLine("WS DISCONNECT!"); return; }
                string answ = GetMessage();
                if (answ[0] == '~') { Console.WriteLine("GET VALUE ERROR!"); return; }
                Console.WriteLine($"{answ}");
                System.Threading.Thread.Sleep(delay);
            }

        }
    }
}
