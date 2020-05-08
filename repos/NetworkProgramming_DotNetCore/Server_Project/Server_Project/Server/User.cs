using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Threading;

namespace Server_Project
{
    class User
    {
        private const int BUF_SIZE = 1024;
        private bool bIsReadStart = false;

        public int ID { get; }

        public Socket Socket { get; set; }
        public Thread ReadingThread { get; set; }
        public List<Socket> Client { get; set; }

        public User(int id, Socket socket, List<Socket> client)
        {
            ID = id;
            Socket = socket;
            Client = client;
            ReadingThread = new Thread(new ThreadStart(ReadMessage));
            bIsReadStart = true;
            ReadingThread.Start();
        }

        public void ReadMessage()
        {
            try
            {
                while (bIsReadStart)
                {
                    byte[] buffer = new byte[BUF_SIZE];
                    Socket.Receive(buffer);

                    foreach (Socket sock in Client)
                    {
                        sock.Send(buffer);
                    }
                }
            }
            catch (SocketException e)
            {
                Console.WriteLine($"{e.Message}");
            }
        }

        public void Quit()
        {
            bIsReadStart = false;
        }
    }
}
