using System;
using System.Text;
using Nettention.Proud;
using SendText;

namespace ProudMyClient
{
    class Program
    {
        public static System.Guid version = new System.Guid("{ 0x3ae33249, 0xecc6, 0x4980, { 0xbc, 0x5d, 0x7b, 0xa, 0x99, 0x9c, 0x7, 0x39 } }");

        public static bool bIsConntected = false;

        public static Proxy Proxy;
        public static Stub Stub;

        public static void JoinServerCompleteHandler(ErrorInfo errorInfo, ByteArray replyFromServer)
        {
            if (errorInfo.errorType == ErrorType.Ok)
            {
                Console.WriteLine($"Connected Completed!");
                bIsConntected = true;

                string userID = "qwer";
                string userPW = "1234";

                Proxy.Login(HostID.HostID_Server, RmiContext.ReliableSend, userID, userPW);
            }
            else
            {
                Console.WriteLine("Failed to connect from server");
            }
        }


        public static void ReceiveMessage(HostID sender, RmiContext rmiContext, ByteArray payload)
        {
            string recvMsg = Encoding.UTF8.GetString(payload.data);
            Console.WriteLine($"Recv from server : {recvMsg}");
        }

        static bool SendText(HostID id, RmiContext rmi, string text)
        {

            return true;
        }


        static void Main(string[] args)
        {
            NetClient client = new NetClient();

            NetConnectionParam param = new NetConnectionParam();

            param.serverIP = "127.0.0.1";
            param.serverPort = 5000;
            param.protocolVersion = new Nettention.Proud.Guid(version);

            Proxy = new Proxy();
            Stub = new Stub();

            client.AttachProxy(Proxy);
            client.AttachStub(Stub);

            client.Connect(param);


            while (true)
            {
                Proxy.Login(HostID.HostID_Server, RmiContext.ReliableSend, "123", "456");
            }

            /*
            NetConnectionParam param = new NetConnectionParam();

            param.serverIP = "127.0.0.1";
            param.serverPort = 5000;
            param.protocolVersion = new Nettention.Proud.Guid(version);

            client.Connect(param);

            client.JoinServerCompleteHandler = JoinServerCompleteHandler;
            client.ReceivedUserMessageHandler = ReceiveMessage;

            while (true)
            {
                if (bIsConntected)
                {
                    ConsoleKeyInfo keyInfo = Console.ReadKey();

                    if (Console.KeyAvailable)
                    {
                        byte[] sendBuf = Encoding.UTF8.GetBytes(keyInfo.Key.ToString());
                        ByteArray payload = new ByteArray(sendBuf);
                        client.SendUserMessage(HostID.HostID_Server, RmiContext.FastEncryptedReliableSend, payload);
                    }
                }

                client.FrameMove();
                */
        }
    }
}
