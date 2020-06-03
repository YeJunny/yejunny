using System;
using Nettention.Proud;
using SendText;

namespace ProudMyServer
{
    class Program
    {
        public static System.Guid version = new System.Guid("{ 0x3ae33249, 0xecc6, 0x4980, { 0xbc, 0x5d, 0x7b, 0xa, 0x99, 0x9c, 0x7, 0x39 } }");

        public static Proxy Proxy;
        public static Stub Stub;


        static void Main(string[] args)
        {
            NetServer server = new NetServer();

            StartServerParameter Param = new StartServerParameter();

            Param.tcpPorts.Add(5000);
            Param.udpPorts.Add(5000);
            Param.protocolVersion = new Nettention.Proud.Guid(version);

            Proxy = new Proxy();
            Stub = new Stub();

            server.AttachProxy(Proxy);
            server.AttachStub(Stub);

            server.Start(Param);

            Console.WriteLine("Start Server Successful!!!");

            while (true)
            {
                
            }
        }
    }
}
