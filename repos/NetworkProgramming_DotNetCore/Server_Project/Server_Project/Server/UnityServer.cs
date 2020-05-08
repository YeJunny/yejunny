using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Reflection;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading;
using System.Xml.Serialization;

namespace Server_Project
{
    [Serializable]
    public class PacketHeader
    {
        public int Type { get; set; }
        public int Length { get; set; }

        public PacketHeader()
        {

        }
        public PacketHeader(int type, int length)
        {
            Type = type;
            Length = length;
        }
    }

    [Serializable]
    public class UserPacket
    {
        public int Index { get; set; }
        public float X { get; set; }
        public float Y { get; set; }
        public float Z { get; set; }
        public float RotX { get; set; }
        public float RotY { get; set; }
        public float RotZ { get; set; }
        public float ScaleX { get; set; }
        public float ScaleY { get; set; }
        public float ScaleZ { get; set; }


        public UserPacket()
        {

        }
    }

    class UnityServer
    {
        public const int BUF_SIZE = 1024;

        //static void Main(string[] args)
        void Main()
        {
            List<Socket> inputedList = new List<Socket>();
            Dictionary<int, Socket> userData = new Dictionary<int, Socket>();

            int serverPort = 5000;
            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Any, serverPort);

            Socket serverSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            serverSocket.Bind(serverEndPoint);
            serverSocket.Listen(5);

            Console.WriteLine($"-------------------------------------------------------------------------");
            Console.WriteLine($" Start Server ... IP : {serverEndPoint.Address}, Port : {serverEndPoint.Port}");
            Console.WriteLine($"-------------------------------------------------------------------------");

            while (true)
            {
                List<Socket> acceptList = new List<Socket>(inputedList);
                acceptList.Add(serverSocket);

                // Check Changed Description Data Until Timeout 1s....
                Socket.Select(acceptList, null, null, 1000000);

                foreach (Socket socket in acceptList)
                {
                    if (socket == serverSocket)
                    {
                        Socket commSocket = socket.Accept();

                        inputedList.Add(commSocket);

                        byte[] buffer = new byte[1024];
                        commSocket.Receive(buffer);
                        string indexStr = Encoding.UTF8.GetString(buffer);
                        int index = int.Parse(indexStr);
                        userData.Add(index, commSocket);

                        Console.WriteLine($"Connected {index}...");
                    }
                    else
                    {
                        try
                        {
                            byte[] headerBuffer = new byte[8];
                            socket.Receive(headerBuffer);
                            DeserializeHeader(headerBuffer);

                            byte[] buffer = new byte[BUF_SIZE];
                            socket.Receive(buffer);
                            //Deserialize(buffer);

                            if (userData.ContainsKey(2))
                            {
                                userData[2].Send(headerBuffer);
                                userData[2].Send(buffer);
                            }
                            if (userData.ContainsKey(1))
                            {
                                userData[1].Send(headerBuffer);
                                userData[1].Send(buffer);
                            }
                        }
                        catch (SocketException e)
                        {
                            Console.WriteLine($"{e.Message}");
                            goto Finish;
                        }
                    }
                }
            }

        Finish:
            Console.WriteLine($"Server End...");
        }

        public static byte[] SerializeHeader(PacketHeader packetHeader)
        {
            byte[] buffer = new byte[8];

            byte[] type = BitConverter.GetBytes(packetHeader.Type);
            byte[] length = BitConverter.GetBytes(packetHeader.Length);

            type.CopyTo(buffer, 0);
            length.CopyTo(buffer, 4);

            return buffer;
        }

        public static PacketHeader DeserializeHeader(byte[] buffer)
        {
            int type = BitConverter.ToInt32(buffer, 0);
            int length = BitConverter.ToInt32(buffer, 4);

            PacketHeader packetHeader = new PacketHeader(type, length);
            return packetHeader;
        }

        public static byte[] Serialize(object packetData)
        {
            BinaryFormatter formatter = new BinaryFormatter();
            MemoryStream stream = new MemoryStream();
            formatter.Serialize(stream, packetData);
            byte[] buffer = stream.ToArray();
            stream.Close();
            return buffer;
        }

        public static object Deserialize(byte[] buffer)
        {
            BinaryFormatter formatter = new BinaryFormatter();
            MemoryStream stream = new MemoryStream(buffer);
            object userData = formatter.Deserialize(stream);
            stream.Close();
            return userData;
        }
    }
}
