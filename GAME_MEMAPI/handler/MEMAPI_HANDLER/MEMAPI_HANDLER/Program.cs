using System;
using System.IO.Ports;


namespace MEMAPI_HANDLER
{
    class Program
    {
        static void Main(string[] args)
        {
            string messageToSend = "Hello";
            string expectedResponse = "hi";
            string foundPort = null;

            string[] portNames = SerialPort.GetPortNames();

            foreach (string portName in portNames)
            {
                using (SerialPort port = new SerialPort(portName))
                {
                    try
                    {
                        port.Open();
                        port.Write(messageToSend);
                        System.Threading.Thread.Sleep(100); // Дайте время на обработку и ответ

                        string response = port.ReadExisting();
                        if (response.Contains(expectedResponse))
                        {
                            foundPort = portName;
                            break; // Выход из цикла, если найден нужный порт
                        }
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine($"Error communicating with {portName}: {ex.Message}");
                    }
                }
            }

            if (!string.IsNullOrEmpty(foundPort))
            {
                Console.WriteLine($"Found the 'hi' response on port: {foundPort}");
            }
            else
            {
                Console.WriteLine("No port with 'hi' response found.");
            }
        }
    }
}
