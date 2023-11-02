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
        class MyObject
        {
            [JsonProperty("string")]
            public string MyString { get; set; }
        }

        static string GetHelloJSON()
        {
            var jsonObject = new
            {
                mode = "hello",
                //offsets = new[] { "0x2C0" },
            };

            string json = JsonConvert.SerializeObject(jsonObject, Formatting.Indented);
            return json;
        }

        static string GetSetValueJSON(string _value)
        {
            var jsonObject = new
            {
                mode = "print",
                value = _value,
                //offsets = new[] { "0x2C0" },
            };

            string json = JsonConvert.SerializeObject(jsonObject, Formatting.Indented);
            return json;
        }
        static string GetSetValueWSTRJSON(string _descr, string _value)
        {
            var jsonObject = new
            {
                mode = "print_wstr",
                descr = _descr,
                value = _value,
                //offsets = new[] { "0x2C0" },
            };

            string json = JsonConvert.SerializeObject(jsonObject, Formatting.Indented);
            return json;
        }
        static string GetSetValueWSTRJSON(string _descr, string _value, int _row)
        {
            var jsonObject = new
            {
                mode = "print_wstr",
                descr = _descr,
                value = _value,
                row = _row,
                //offsets = new[] { "0x2C0" },
            };

            string json = JsonConvert.SerializeObject(jsonObject, Formatting.Indented);
            return json;
        }

    }
}
