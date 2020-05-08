using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace Server_Project
{
    class ChatingServer
    {
        private bool bIsAcceptStart = false;

        public Socket Socket { get; set; }
        public ChatingRoom Room { get; set; }
        public Thread Thread { get; set; }

        public ChatingServer(int backlogNum, int port, string roomName)
        {
            Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPEndPoint servPoint = new IPEndPoint(IPAddress.Any, port);
            Socket.Bind(servPoint);
            Socket.Listen(backlogNum);

            Room = new ChatingRoom(roomName);
            Thread = new Thread(new ThreadStart(Accept));
            bIsAcceptStart = true;
            Thread.Start();
        }

        public void Accept()
        {
            try
            {
                while (bIsAcceptStart)
                {
                    Socket clntSocket = Socket.Accept();
                    Room.AddClient(clntSocket);
                    Room.Start();
                }
            }
            catch (SocketException e)
            {
                Console.WriteLine($"{e.Message}");
            }
        }

        public void PrintUserAll()
        {
            Room.PrintUserAll();
        }

        public void Close()
        {
            bIsAcceptStart = false;
            Room.Quit();
            Socket.Close();
        }
    }
}
