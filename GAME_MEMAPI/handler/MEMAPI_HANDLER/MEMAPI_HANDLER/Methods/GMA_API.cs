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
        static string GetPointerJSON(string module, int base_offset, List<int> offsets)
        {
            var jsonObject = new
            {
                mode = "get_pointer",
                module = $"{module}",
                base_offset = $"{base_offset}",
                offsets = offsets.ToArray(),
                //offsets = new[] { "0x2C0" },
            };

            string json = JsonConvert.SerializeObject(jsonObject, Formatting.Indented);
            return json;
        }
        static string GetPointerJSON(string module, string base_offset, List<string> offsets)
        {
            var jsonObject = new
            {
                mode = "get_pointer",
                module = $"{module}",
                base_offset = $"{base_offset}",
                offsets = offsets.ToArray(),
                //offsets = new[] { "0x2C0" },
            };

            string json = JsonConvert.SerializeObject(jsonObject, Formatting.Indented);
            return json;
        }
        static string GetGetValueJSON(string pointer, string type)
        {
            var jsonObject = new
            {
                mode = "get_value",
                pointer = $"{pointer}",
                type = $"{type}",
                //offsets = new[] { "0x2C0" },
            };

            string json = JsonConvert.SerializeObject(jsonObject, Formatting.Indented);
            return json;
        }
        static string GetSetValueJSON(string pointer, string type, string value)
        {
            var jsonObject = new
            {
                mode = "set_value",
                pointer = $"{pointer}",
                type = $"{type}",
                value = $"{value}",
                //offsets = new[] { "0x2C0" },
            };

            string json = JsonConvert.SerializeObject(jsonObject, Formatting.Indented);
            return json;
        }
        static string GetGetModuleJSON(string module)
        {
            var jsonObject = new
            {
                mode = "get_module",
                module = $"{module}",
                //offsets = new[] { "0x2C0" },
            };

            string json = JsonConvert.SerializeObject(jsonObject, Formatting.Indented);
            return json;
        }
        static string GetGetModulesJSON()
        {
            var jsonObject = new
            {
                mode = "get_modules",
                //offsets = new[] { "0x2C0" },
            };

            string json = JsonConvert.SerializeObject(jsonObject, Formatting.Indented);
            return json;
        }
    }
}
