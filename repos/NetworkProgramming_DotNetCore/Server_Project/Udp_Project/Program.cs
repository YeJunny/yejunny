using Microsoft.VisualBasic.CompilerServices;
using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace Udp_Project
{
    class Program
    {
        static void Main(string[] args)
        {
            int port = 6000;

            IPEndPoint servPoint = new IPEndPoint(IPAddress.Any, port);

            Socket servSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

            servSocket.Bind(servPoint);

            Console.WriteLine("========= SERVER ON ===========");

            EndPoint remotePoint = (EndPoint)new IPEndPoint(IPAddress.Any, 0);

            while (true)
            {
                byte[] x = new byte[4];
                byte[] y = new byte[4];
                byte[] z = new byte[4];
                servSocket.ReceiveFrom(x, ref remotePoint);
                servSocket.ReceiveFrom(y, ref remotePoint);
                servSocket.ReceiveFrom(z, ref remotePoint);

                Console.WriteLine($"Recv Data!");
                Console.WriteLine($"Remote IP : {((IPEndPoint)remotePoint).Address}, Remote Port : {((IPEndPoint)remotePoint).Port}");
                
                Console.WriteLine($"x : {BitConverter.ToSingle(x)}");
                Console.WriteLine($"y : {BitConverter.ToSingle(y)}");
                Console.WriteLine($"z : {BitConverter.ToSingle(z)}");

                string sendStr = "Receviced Server!";
                byte[] sendBuffer = Encoding.UTF8.GetBytes(sendStr);

                servSocket.SendTo(sendBuffer, remotePoint);
            }

            servSocket.Close();
        }

    }
}
