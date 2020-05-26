using Nettention.Proud;
using System;
using System.Text;

namespace ProudMyServer
{
    class Server
    {
        private string mGuid = "{0xa90a5fc4,0x2b62,0x46f8,{0xbb,0xc4,0xf6,0xea,0xc1,0xef,0x70,0x26}}";

        public StartServerParameter Param { private set; get; }
        public NetServer Serv { private set; get; }
        private SpawnManager mSpawnManger;
        private int HP = 20;

        private void ClientJoinHandler(NetClientInfo clientInfo)
        {
            Console.WriteLine($"Connected Client : {clientInfo.hostID}");
            Spawn();
        }

        private void ClientLeaveHandler(NetClientInfo clientInfo, ErrorInfo errorInfo, ByteArray comment)
        {
            Console.WriteLine($"Leaved Client : {clientInfo.hostID}");
        }

        private void ReceiveUserMessage(HostID sender, RmiContext rmiContext, ByteArray payload)
        {
            int id = BitConverter.ToInt32(payload.data);

            if (id == 101)
            {
                HP -= 10;

                if (HP <= 0)
                {
                    id = 102;
                    byte[] buf = BitConverter.GetBytes(id);
                    ByteArray sendPayload = new ByteArray(buf);
                    Serv.SendUserMessage(sender, RmiContext.ReliableSend, sendPayload);
                }
            }
        }

        public Server(ushort port)
        {
            Serv = new NetServer();
            mSpawnManger = new SpawnManager(Serv);

            Param = new StartServerParameter();

            Param.protocolVersion = new Nettention.Proud.Guid(mGuid);
            Param.tcpPorts.Add(port);

            Serv.ClientJoinHandler = ClientJoinHandler;
            Serv.ClientLeaveHandler = ClientLeaveHandler;
        }

        public void Start()
        {
            Serv.Start(Param);

            mSpawnManger.Start();
        }

        public void Run()
        {
            
        }

        public void Spawn()
        {
            int hostID = 100;

            HostID[] hostIDs = Serv.GetClientHostIDs();

            foreach (HostID id in hostIDs)
            {
                ByteArray payload = new ByteArray(BitConverter.GetBytes(hostID));
                Serv.SendUserMessage(id, RmiContext.ReliableSend, payload);
            }
        }
    }
}
