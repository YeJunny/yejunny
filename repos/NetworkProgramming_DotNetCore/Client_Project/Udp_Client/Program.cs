using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace Udp_Client
{
    class Program
    {
        static void Main(string[] args)
        {
            string ip = "127.0.0.1";
            int port = 6000;

            IPEndPoint servPoint = new IPEndPoint(IPAddress.Parse(ip), port);
            Socket clntSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

            string sendStr = "Client to Server";
            byte[] sendBuffer = Encoding.UTF8.GetBytes(sendStr);
            clntSocket.SendTo(sendBuffer, (EndPoint)servPoint);

            EndPoint remotePoint = new IPEndPoint(IPAddress.Any, 0);

            byte[] recvBuffer = new byte[1024];
            clntSocket.ReceiveFrom(recvBuffer, ref remotePoint);

            Console.WriteLine($"Recv Data!");
            Console.WriteLine($"Remote IP : {((IPEndPoint)remotePoint).Address}, Remote Port : {((IPEndPoint)remotePoint).Port}");
            Console.WriteLine($"Message : {Encoding.UTF8.GetString(recvBuffer)}");

            clntSocket.Close();
        }
    }
}
