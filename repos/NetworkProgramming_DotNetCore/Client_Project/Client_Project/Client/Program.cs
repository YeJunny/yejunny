using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace Client_Project
{
    class Program
    {
        public static void Main(string[] args)
        {
            ChatingClient client = new ChatingClient("127.0.0.1", 5000);
            client.Start();

            while (true)
            {
                client.WritingThread.Join();
                break;
            }

            client.Close();
        }
    }
}
