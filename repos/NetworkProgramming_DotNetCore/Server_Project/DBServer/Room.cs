using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Threading;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace DBServer
{
    struct UserInfo
    {
        public GameDTO User;
        public Socket Socket;
    }

    class Room
    {
        private bool bIsStart = false;

        public List<UserInfo> UserList { get; set; }
        public Thread Thread { get; set; }
        public string RoomName { get; set; }

        public Room(string roomName)
        {
            UserList = new List<UserInfo>();
            RoomName = roomName;
            Thread = new Thread(new ThreadStart(Run));
            bIsStart = true;
            Thread.Start();
        }

        public void AddClient(GameDTO user, Socket socket)
        {
            UserInfo userInfo = new UserInfo();
            userInfo.User = user;
            userInfo.Socket = socket;

            UserList.Add(userInfo);
        }

        public void Run()
        {
            while (bIsStart)
            {
                try
                {
                    List<Socket> checkList = new List<Socket>();
                    List<string> leavedUsers = new List<string>();

                    foreach (UserInfo userInfo in UserList.ToArray())
                    {
                        if (!userInfo.Socket.Connected)
                        {
                            leavedUsers.Add(userInfo.User.ID);
                            UserList.Remove(userInfo);
                        }
                        else
                        {
                            checkList.Add(userInfo.Socket);
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

                    if (checkList.Count > 0)
                    {
                        Socket.Select(checkList, null, null, 1000);
                    }

                    if (checkList.Count > 0)
                    {
                        byte[] buffer = new byte[1024];
                        foreach (Socket socket in checkList)
                        {
                            socket.Receive(buffer);

                            foreach (UserInfo user in UserList)
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

        public void PrintUserAll()
        {
            foreach (UserInfo userInfo in UserList)
            {
                IPEndPoint userPoint = (IPEndPoint)userInfo.Socket.RemoteEndPoint;
                Console.WriteLine("================================================================");
                Console.WriteLine($"User ID : {userInfo.User.ID}, Name : {userInfo.User.Name}, Email : {userInfo.User.Email}");
                Console.WriteLine($"User IP : {userPoint.Address}, Port {userPoint.Port}");
            }
        }

        public void Quit()
        {
            bIsStart = false;

            foreach (UserInfo user in UserList)
            {
                user.Socket.Close();
            }

            try
            {
                Thread.Join();
            }
            catch (SocketException e)
            {
                Console.WriteLine($"Exception : Your Selection is Exit.\nNot Started Thread");
            }
        }
    }
}
