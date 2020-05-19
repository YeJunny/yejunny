using System;
using Nettention.Proud;

namespace ProudNetServer
{
    class Program
    {
        enum EMenu
        {
            ServerMenu,
            ClientMenu,
            Exit
        };

        static System.Guid guid = new System.Guid("{ 0x3ae33249, 0xecc6, 0x4980, { 0xbc, 0x5d, 0x7b, 0xa, 0x99, 0x9c, 0x7, 0x39 } }");

        static void Main(string[] args)
        {
            NetServer server = new NetServer();
            StartServerParameter param = new StartServerParameter();

            param.tcpPorts.Add(5000);
            param.udpPorts.Add(5000);

            param.protocolVersion = new Nettention.Proud.Guid(guid);

            server.Start(param);


            while (true)
            {
                Console.WriteLine("Select : ");
                EMenu menu = (EMenu)int.Parse(Console.ReadLine());

                switch (menu)
                {
                    case EMenu.Exit:
                        break;
                }
            }
        }
    }
}
