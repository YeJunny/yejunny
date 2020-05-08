using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Security.Cryptography;
using System.Threading;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace DBClient
{
    [Serializable]
    class GameDTO
    {
        public string ID { get; set; }
        public string Password { get; set; }
        public string Name { get; set; }
        public string Email { get; set; }

        public GameDTO(string id, string hashPassword, string name, string email)
        {
            ID = id;
            Password = hashPassword;
            Name = name;
            Email = email;
        }
    }

    class DBClient
    {
        private const int BUF_SIZE = 1024;

        private enum ERead
        {
            Leave,
            Read
        };

        public GameDTO User { get; set; }
        public Socket Socket { get; set; }
        public Thread WritingThread { get; set; }
        public Thread ReadingThread { get; set; }

        public static string GetSha1(string str)
        {
            byte[] data = Encoding.ASCII.GetBytes(str);

            SHA1 sha = new SHA1Managed();
            byte[] result = sha.ComputeHash(data);

            string hash = string.Empty;
            
            foreach (byte ch in result)
            {
                hash += ch.ToString("X2");
            }

            return hash;
        }

        public DBClient()
        {
            Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            ReadingThread = new Thread(new ThreadStart(ReadMessage));
            WritingThread = new Thread(new ThreadStart(WriteMessage));
        }

        public bool Register(string id, string password, string name, string email)
        {
            Socket.Connect(new IPEndPoint(IPAddress.Parse("127.0.0.1"), 5000));

            string hashPassword = GetSha1(password);

            string regData = $"0;{id};{hashPassword};{name};{email};";

            byte[] regBuffer = Encoding.UTF8.GetBytes(regData);
            Socket.Send(regBuffer);

            byte[] recvBuffer = new byte[4];
            Socket.Receive(recvBuffer);
            int recvNum = BitConverter.ToInt32(recvBuffer);

            Socket.Close();


            if (recvNum == 1)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        public bool Login(string id, string password, int roomNum)
        {
            Socket.Connect(new IPEndPoint(IPAddress.Parse("127.0.0.1"), 5000));

            string hashPassword = GetSha1(password);

            string loginData = $"1;{id};{hashPassword};{roomNum};";

            byte[] loginBuffer = Encoding.UTF8.GetBytes(loginData);
            Socket.Send(loginBuffer);

            byte[] recvBuffer = new byte[4];
            Socket.Receive(recvBuffer);

            int recvNum = BitConverter.ToInt32(recvBuffer);

            if (recvNum == 1)
            {
                byte[] userBuffer = new byte[BUF_SIZE];
                Socket.Receive(userBuffer);

                MemoryStream stream = new MemoryStream(userBuffer);
                BinaryFormatter formatter = new BinaryFormatter();
                List<string> userData = (List<string>)formatter.Deserialize(stream);

                User = new GameDTO(userData[0], null, userData[1], userData[2]);

                WritingThread.Start();
                ReadingThread.Start();

                return true;
            }
            else
            {
                Socket.Close();

                return false;
            }
        }

        public void ReadMessage()
        {
            try
            {
                while (true)
                {
                    byte[] buffer = new byte[BUF_SIZE];
                    Socket.Receive(buffer);

                    ERead headerNum = (ERead)BitConverter.ToInt32(buffer);
                    switch (headerNum)
                    {
                        case ERead.Leave:
                            {
                                Socket.Receive(buffer);
                                MemoryStream stream = new MemoryStream(buffer);
                                BinaryFormatter formatter = new BinaryFormatter();
                                List<string> leavedList = (List<string>)formatter.Deserialize(stream);

                                foreach (string id in leavedList)
                                {
                                    Console.WriteLine($"Leaved {id}...");
                                }

                                break;
                            }

                        case ERead.Read:
                            {
                                Socket.Receive(buffer);
                                string msg = Encoding.UTF8.GetString(buffer).Split('\0')[0];
                                string[] strBuffer = msg.Split(';');
                                string id = strBuffer[0];
                                string message = strBuffer[1];
                                
                                if (id != User.ID)
                                {
                                    Console.WriteLine($"{id} : {message}");
                                }

                                break;
                            }
                    }
                }
            }
            catch (SocketException e)
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
                    string message = $"{User.ID};{Console.ReadLine()};";
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
