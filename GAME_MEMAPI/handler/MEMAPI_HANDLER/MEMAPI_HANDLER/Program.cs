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






        /*public static string GMA_START_MSG = "HI";
        public static string GMA_DEV_RESPONSE_MSG = "HI_GMA";*/
        public static string ARDUION_PROJECT = "veh_device";
        public static bool USE_ARDUINO_COM = true;


        static void Main(string[] args)
        {
            // { "mode":"get_pointer", "module" : "fmodstudio.dll", "base_offset" : "0x158B50", "offsets" : ["0x5C0", "0x8", "0x60", "0x60", "0x18", "0x208"] }
            //string foundPort = FindCOMPort(GMA_START_MSG, GMA_DEV_RESPONSE_MSG);
            //if (foundPort == "") { Console.WriteLine($"No port with '{GMA_DEV_RESPONSE_MSG}' response found."); return; }

            //test(); return;

            string COM = "";
            if(USE_ARDUINO_COM)
            {
                for (int i = 0; i < 5; i++) { COM = FindCOMPort(ARDUION_PROJECT); if (COM == "") { System.Threading.Thread.Sleep(1000); } else { break; } }
                if (COM == "") { Console.WriteLine("COM NOT FOUND"); return; }
                Console.WriteLine($"COM: {COM}");
            }


            int GMA_PORT = 9005;
            //int delay = 1 * 1000;
            int delay = 1000;
            bool status_connect = false;
            //if (!status_connect) { Console.WriteLine($"WS CANT CONNECT TO PORT {GMA_PORT}"); return; }

            string json = GetGetPointerJSON("gta3.exe", "0x5412F0", new List<string> { "0x2C0" }); // x86 test
            json = GetGetPointerJSON("fmodstudio.dll", "0x158B50", new List<string> { "0x5C0", "0x8", "0x60", "0x60", "0x18", "0x208" }); // x64 test

            Console.WriteLine(json + "\n");
            //json = GetGetValueJSON("0x0", "float");

            string calc_pointer = "";
            while (true)
            {
                //if (!UWSGetState()) { UWSConect(GMA_PORT); continue; }
                //if (!UWSSend(json)) { Console.WriteLine("WS COUD NOT CONNECT!"); System.Threading.Thread.Sleep(delay); continue; }
                (bool, string) answ = UWSSend(json);
                if ((!answ.Item1) || ((answ.Item2.Length > 0) && (answ.Item2[0] == '~'))) { Console.WriteLine("MEM CHAIN POINTERS NOT INIT!"); System.Threading.Thread.Sleep(delay); continue; }
                Console.WriteLine($"POINTER: {answ.Item2}");
                //SendMsgComNoResp(COM, GetSetValueJSON("STOP"));
                calc_pointer = answ.Item2;
                break;
            }


            json = GetGetValueJSON(calc_pointer, "float");
            while (true)
            {
                Console.WriteLine($"GETTING UWS");
                //if ((!UWSSend(json)) || (!UWSGetState())) { Console.WriteLine("WS DISCONNECT!"); /*return;*/ }
                (bool, string) answ = UWSSend(json);
                if ((!answ.Item1) || ((answ.Item2.Length > 0) && (answ.Item2[0] == '~'))) { Console.WriteLine("GET VALUE ERROR!"); return; }
                int val = (int)float.Parse(answ.Item2.Replace(".", ","));
                Console.WriteLine($"{answ.Item2}");

                if(USE_ARDUINO_COM)
                {
                    Console.WriteLine($"SENDING TO ARDUINO!");
                    SendMsgComNoResp(COM, GetSetValueWSTRJSON("speed", $"{val} km/h"));
                }

                System.Threading.Thread.Sleep(delay);
            }

        }

        static void test()
        {
            string json = GetFindPatternJSON("7FF71890C000", "10 10 10 10 00 00 ? ? ? ? 20 20", "500");
            json = GetFindPatternJSON("5e5000", "80 F0 FF FF FF ? ? ? 80 10 ?", "500");
            //json = "{\"mode\":\"find_pattern\",\"pointer\":\"7FF71890C000\",\"pattern\":\"10 10 10 10 00 00 ? ? ? ? 20 20\",\"block_size\":\"500\"}";
            //json = "{\"mode\":\"get_value\",\"pointer\":\"7FF71890C000\",\"type\":\"int\"}";
            (bool, string) answ = UWSSend(json);
            Console.WriteLine($"{answ.Item2}");
        }
    }
}
