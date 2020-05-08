using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Threading;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace Client_Project
{
    class ChatingClient
    {
        private const int BUF_SIZE = 1024;
        private IPEndPoint mServPoint;
        private int mID;

        public Socket Socket { get; set; }
        public Thread WritingThread { get; set; }
        public Thread ReadingThread { get; set; }

        public ChatingClient(string ip, int port)
        {
            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse(ip), port);
            Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            mServPoint = new IPEndPoint(IPAddress.Parse(ip), port);
            ReadingThread = new Thread(new ThreadStart(ReadMessage));
            WritingThread = new Thread(new ThreadStart(WriteMessage));
        }

        public void Start()
        {
            while (true)
            {
                try
                {
                    Socket.Connect(mServPoint);
                    byte[] idBuf = new byte[4];
                    Socket.Receive(idBuf);
                    mID = BitConverter.ToInt32(idBuf);
                    break;
                }
                catch (SocketException e)
                {
                    Console.WriteLine($"Connection Error!");
                    Console.WriteLine($"Target IP : {mServPoint.Address}, Port : {mServPoint.Port}");
                    Console.WriteLine($"Error Content : {e.Message}");
                    Console.WriteLine($"Try Re-connect...");
                }
            }

            Console.WriteLine($"====================================================");
            Console.WriteLine($" Clinet connect to Server");
            Console.WriteLine($" connected Server IP : {mServPoint.Address}, Port : {mServPoint.Port}");
            Console.WriteLine($" My ID : {mID}");
            Console.WriteLine($" Connection Successful!");
            Console.WriteLine($"====================================================");
            ReadingThread.Start();
            WritingThread.Start();
        }

        public void Close()
        {
            Socket.Close();
        }

        public void ReadMessage()
        {
            try
            {
                while (true)
                {
                    byte[] buffer = new byte[BUF_SIZE];
                    Socket.Receive(buffer);

                    int headerNum = BitConverter.ToInt32(buffer);
                    switch (headerNum)
                    {
                        case -1:
                            {
                                Socket.Receive(buffer);
                                MemoryStream stream = new MemoryStream(buffer);
                                BinaryFormatter formatter = new BinaryFormatter();
                                List<int> leavedList = (List<int>)formatter.Deserialize(stream);

                                foreach(int id in leavedList)
                                {
                                    Console.WriteLine($"Leaved User{id}...");
                                }

                                break;
                            }

                        case 1:
                            {
                                Socket.Receive(buffer);
                                string msg = Encoding.UTF8.GetString(buffer).Split('\0')[0];
                                string[] strBuffer = msg.Split(',');
                                int id;
                                if (int.TryParse(strBuffer[0], out id))
                                {
                                    string message = strBuffer[1];
                                    if (id != mID)
                                    {
                                        Console.WriteLine($"User{id} : {message}");
                                    }
                                }
                                else
                                {
                                    Console.WriteLine($"{msg}");
                                }

                                break;
                            }
                    }
                }
            }
            catch(SocketException e)
            {
                Console.WriteLine("Disconnection from Server...");
                Console.WriteLine($"Error Message : {e.Message}");
            }
        }

        public void WriteMessage()
        {
            try
            {
                while (true)
                {
                    string message = $"{mID},{Console.ReadLine()}";
                    byte[] buffer = Encoding.UTF8.GetBytes(message);
                    Socket.Send(buffer);
                }
            }
            catch (SocketException e)
            {
                Console.WriteLine("Disconnection from Server...");
                Console.WriteLine($"Error Message : {e.Message}");
            }
}
    }
}
