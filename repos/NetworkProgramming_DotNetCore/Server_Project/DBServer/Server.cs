using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Threading;
using System.Security.Cryptography;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace DBServer
{
    class Server
    {
        private enum EThreadName
        {
            Accept,
            DB
        };

        private enum EMenu
        {
            Register,
            Login
        };

        private bool bIsAcceptStart = false;

        public List<Room> Rooms { get; set; }
        public Thread Thread { get; set; }
        public Socket ServerSocket { get; set; }
        public Socket ClientSocket { get; set; }
        public GameDAO DB { get; set; }

        public Server(int backlogNum, int port)
        {
            ServerSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            ServerSocket.Bind(new IPEndPoint(IPAddress.Any, port));
            ServerSocket.Listen(backlogNum);

            Rooms = new List<Room>();
            Thread = new Thread(new ThreadStart(Accept));
            DB = new GameDAO();
            bIsAcceptStart = true;
            Thread.Start();
        }

        public void Accept()
        {
            try
            {
                while (bIsAcceptStart)
                {
                    Socket clntSocket = ServerSocket.Accept();

                    // Login
                    byte[] accountData = new byte[1024];
                    clntSocket.Receive(accountData);
                    string[] accountStr = Encoding.UTF8.GetString(accountData).Split(';');
                    EMenu select = (EMenu)(int.Parse(accountStr[0]));

                    switch (select)
                    {
                        case EMenu.Register:
                            {
                                string id = accountStr[1];
                                string hashedPassword = accountStr[2];
                                string name = accountStr[3];
                                string email = accountStr[4];

                                if (DB.Register(new GameDTO(id, hashedPassword, name, email)))
                                {
                                    byte[] okBuf = BitConverter.GetBytes(1);
                                    clntSocket.Send(okBuf);
                                }
                                else
                                {
                                    byte[] failBuf = BitConverter.GetBytes(-1);
                                    clntSocket.Send(failBuf);
                                }

                                continue;
                            }

                        case EMenu.Login:
                            {
                                string id = accountStr[1];
                                string hashPassword = accountStr[2];
                                int roomNum;
                                int.TryParse(accountStr[3], out roomNum);

                                GameDTO user = DB.Login(id, hashPassword);
                                if (user == null || roomNum > Rooms.Count || roomNum == null)
                                {
                                    byte[] failBuf = BitConverter.GetBytes(-1);
                                    clntSocket.Send(failBuf);

                                    continue;
                                }
                                else
                                {
                                    byte[] okBuf = BitConverter.GetBytes(1);
                                    clntSocket.Send(okBuf);

                                    List<string> userData = new List<string>();
                                    userData.Add(user.ID);
                                    userData.Add(user.Name);
                                    userData.Add(user.Email);

                                    MemoryStream stream = new MemoryStream();
                                    BinaryFormatter formatter = new BinaryFormatter();
                                    formatter.Serialize(stream, userData);

                                    byte[] userBuf = stream.ToArray();
                                    clntSocket.Send(userBuf);

                                    // Console
                                    IPEndPoint clientPoint = (IPEndPoint)clntSocket.RemoteEndPoint;
                                    Console.WriteLine("=========================================");
                                    Console.WriteLine("Client Connection!");
                                    Console.WriteLine($"User ID : {user.ID}, Name : {user.Name}, Email : {user.Email}");
                                    Console.WriteLine($"IP : {clientPoint.Address}, Port : {clientPoint.Port}");
                                    Console.WriteLine("=========================================");

                                    // Add Client

                                    Rooms[roomNum].AddClient(user, clntSocket);

                                    break;
                                }
                            }

                        default:
                            {
                                byte[] failBuf = BitConverter.GetBytes(-1);
                                clntSocket.Send(failBuf);

                                continue;
                            }
                    }
                }
            }
            catch (SocketException e)
            {
                Console.WriteLine($"{e.Message}");
            }
        }

        public void CreateRoom(string roomName)
        {
            Room room = new Room(roomName);
            Rooms.Add(room);
        }

        public void DeleteRoom(string roomName)
        {
            foreach (Room room in Rooms)
            {
                if (room.RoomName == roomName)
                {
                    room.Quit();
                    Rooms.Remove(room);
                    break;
                }
            }
        }

        public void PrintAllUserAtRoom(string roomName)
        {
            foreach (Room room in Rooms)
            {
                if (room.RoomName == roomName)
                {
                    room.PrintUserAll();
                    break;
                }
            }
        }
    }
}
