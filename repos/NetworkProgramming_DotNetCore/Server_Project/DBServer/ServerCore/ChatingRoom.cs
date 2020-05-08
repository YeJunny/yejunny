using System;
using System.Collections.Generic;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace Server_Project
{
    struct UserInfo
    {
        public int ID { get; set; }
        public Socket Socket { get; set; }
    }

    class ChatingRoom
    {
        private int seed;
        private bool bIsStart = false;

        public List<UserInfo> userList { get; set; }
        public Thread thread { get; set; }
        public string RoomName { get; set; }

        public ChatingRoom(string roomName)
        {
            Random random = new Random();
            seed = random.Next(0, 1231);
            userList = new List<UserInfo>();
            RoomName = roomName;
        }

        public void AddClient(Socket socket)
        {
            UserInfo user = new UserInfo();
            user.ID = seed;
            user.Socket = socket;

            userList.Add(user);

            byte[] id = BitConverter.GetBytes(user.ID);
            socket.Send(id);

            Console.WriteLine("===============================================================");
            Console.WriteLine("Connection successful!");
            Console.WriteLine($"Clinet Info : {((IPEndPoint)socket.RemoteEndPoint).Address}, {((IPEndPoint)socket.RemoteEndPoint).Port}");
            Console.WriteLine("Add Socket...");
            Console.WriteLine("===============================================================");

            ++seed;
        }

        public void Run()
        {
            while (bIsStart)
            {
                try
                {
                    List<Socket> checkList = new List<Socket>();
                    List<int> leavedUsers = new List<int>();

                    foreach (UserInfo user in userList.ToArray())
                    {
                        if (!user.Socket.Connected)
                        {
                            leavedUsers.Add(user.ID);
                            userList.Remove(user);
                        }
                        else
                        {
                            checkList.Add(user.Socket);
                        }
                    }

                    if (leavedUsers.Count > 0)
                    {
                        MemoryStream stream = new MemoryStream();
                        BinaryFormatter formatter = new BinaryFormatter();
                        formatter.Serialize(stream, leavedUsers);
                        byte[] buffer = stream.GetBuffer();

                        int headerNum = -1;
                        byte[] headerBuffer = BitConverter.GetBytes(headerNum);

                        foreach (Socket socket in checkList)
                        {
                            socket.Send(headerBuffer);
                            socket.Send(buffer);
                        }
                    }

                    Socket.Select(checkList, null, null, 1000);

                    if (checkList.Count > 0)
                    {
                        byte[] buffer = new byte[1024];
                        foreach (Socket socket in checkList)
                        {
                            socket.Receive(buffer);
                            
                            foreach (UserInfo user in userList)
                            {
                                int headerNum = 1;
                                byte[] headerBuffer = BitConverter.GetBytes(headerNum);
                                user.Socket.Send(headerBuffer);
                                user.Socket.Send(buffer);
                            }
                        }
                    }
                }
                catch (SocketException e)
                {
                    Console.WriteLine($"{e.Message}");
                }
            }

        }

        public void Start()
        {
            if (!bIsStart)
            {
                thread = new Thread(new ThreadStart(Run));
                bIsStart = true;
                thread.Start();
            }
        }

        public void PrintUserAll()
        {
            foreach (UserInfo user in userList)
            {
                IPEndPoint userPoint = (IPEndPoint)user.Socket.RemoteEndPoint;
                Console.WriteLine("===============================================");
                Console.WriteLine($"User{user.ID} ... IP : {userPoint.Address}, Port {userPoint.Port}");
            }
        }

        public void Quit()
        {
            bIsStart = false;

            foreach (UserInfo user in userList)
            {
                user.Socket.Close();
            }

            try
            {
                thread.Join();
            }
            catch (SocketException e)
            {
                Console.WriteLine($"Exception : Your Selection is Exit.\nNot Started Thread");
            }
        }
    }
}
