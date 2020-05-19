using System;
using System.ComponentModel.Design;
using System.Text;
using System.Threading;
using Nettention.Proud;

namespace ProudMyServer
{
    class Program
    {
        public static System.Guid version = new System.Guid("{ 0x3ae33249, 0xecc6, 0x4980, { 0xbc, 0x5d, 0x7b, 0xa, 0x99, 0x9c, 0x7, 0x39 } }");

        public enum EMenu
        {
            List,
            o,
            q,
            Exit,
        };

        static void Main(string[] args)
        {
            Server server = new Server(5000);

            server.Start();

            server.Spawn();

            while (true)
            {
                server.Run();
            }
        }
    }
}
