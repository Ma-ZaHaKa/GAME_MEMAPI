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
        static string GetGetPointerJSON(string _module, int _base_offset, List<int> offsets)
        {
            var jsonObject = new
            {
                mode = "get_pointer",
                module = $"{_module}",
                base_offset = $"{_base_offset}",
                offsets = offsets.ToArray(),
                //offsets = new[] { "0x2C0" },
            };

            string json = JsonConvert.SerializeObject(jsonObject, Formatting.Indented);
            return json;
        }

        static string GetGetPointerJSON(string _module, string _base_offset, List<string> offsets)
        {
            var jsonObject = new
            {
                mode = "get_pointer",
                module = $"{_module}",
                base_offset = $"{_base_offset}",
                offsets = offsets.ToArray(),
                //offsets = new[] { "0x2C0" },
            };

            string json = JsonConvert.SerializeObject(jsonObject, Formatting.Indented);
            return json;
        }
        static string GetFindPatternJSON(string _pointer, string _pattern, int _block_size)
        {
            var jsonObject = new
            {
                mode = "find_pattern",
                pointer = $"{_pointer}",
                pattern = $"{_pattern}",
                block_size = _block_size,
                //offsets = new[] { "0x2C0" },
            };

            string json = JsonConvert.SerializeObject(jsonObject, Formatting.Indented);
            return json;
        }
        static string GetFindPatternJSON(string _pointer, string _pattern, string _block_size)
        {
            var jsonObject = new
            {
                mode = "find_pattern",
                pointer = $"{_pointer}",
                pattern = $"{_pattern}",
                block_size = $"{_block_size}",
                //offsets = new[] { "0x2C0" },
            };

            string json = JsonConvert.SerializeObject(jsonObject, Formatting.Indented);
            return json;
        }
        static string GetGetValueJSON(string _pointer, string _type)
        {
            var jsonObject = new
            {
                mode = "get_value",
                pointer = $"{_pointer}",
                type = $"{_type}",
                //offsets = new[] { "0x2C0" },
            };

            string json = JsonConvert.SerializeObject(jsonObject, Formatting.Indented);
            return json;
        }

        static string GetSetValueJSON(string _pointer, string _type, string _value)
        {
            var jsonObject = new
            {
                mode = "set_value",
                pointer = $"{_pointer}",
                type = $"{_type}",
                value = $"{_value}",
                //offsets = new[] { "0x2C0" },
            };

            string json = JsonConvert.SerializeObject(jsonObject, Formatting.Indented);
            return json;
        }
        static string GetGetModuleJSON(string _module)
        {
            var jsonObject = new
            {
                mode = "get_module",
                module = $"{_module}",
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
