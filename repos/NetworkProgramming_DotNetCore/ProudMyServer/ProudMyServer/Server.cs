using Nettention.Proud;
using ServerPIDL;
using System;
using System.Collections.Generic;
using System.Text;

namespace ProudMyServer
{
    class Server
    {
        private System.Guid version = new System.Guid("{ 0x3ae33249, 0xecc6, 0x4980, { 0xbc, 0x5d, 0x7b, 0xa, 0x99, 0x9c, 0x7, 0x39 } }");

        private Proxy Proxy;
        private Stub Stub;

        private DAO DAO;

        public Server()
        {
            NetServer server = new NetServer();

            StartServerParameter Param = new StartServerParameter();

            Param.tcpPorts.Add(5000);
            Param.udpPorts.Add(5000);
            Param.protocolVersion = new Nettention.Proud.Guid(version);

            Proxy = new Proxy();
            Stub = new Stub();

            server.AttachProxy(Proxy);
            server.AttachStub(Stub);

            server.ClientJoinHandler = ClientJoinHandler;
            server.ClientLeaveHandler = ClientLeaveHandler;

            Stub.RegisterRequest = RegisterRequest;
            Stub.LoginRequest = LoginRequest;
            Stub.CreatePlayer = CreatePlayerRequest;
            Stub.GetPlayerRequest = GetPlayerRequest;

            server.Start(Param);
            DAO = new DAO();

            Console.WriteLine("Start Server Successful!!!");
        }

        public void ClientJoinHandler(NetClientInfo clientInfo)
        {
            Console.WriteLine($"Client Join Handler : {clientInfo.hostID}");
        }

        public void ClientLeaveHandler(NetClientInfo clientInfo, ErrorInfo errorinfo, ByteArray comment)
        {
            Console.WriteLine($"Client Leave Handler : {clientInfo.hostID}");
        }

        public bool RegisterRequest(HostID hostID, RmiContext rmiContext, string id, string pw, string name)
        {
            return DAO.Register(new AccountDTO(id, pw, name));
        }

        public bool LoginRequest(HostID hostID, RmiContext rmiContext, string id, string pw)
        {
            return DAO.Login(new AccountDTO(id, pw, null));
        }

        public bool CreatePlayerRequest(HostID hostID, RmiContext rmiContext, string id, string tag)
        {
            PlayerDTO player = DAO.CreatePlayer(new PlayerDTO(id, tag, 0, 0, 0));

            if (player == null)
            {
                return false;
            }

            Proxy.GetPlayerResponse(hostID, rmiContext, player.UserID, player.Tag, player.Bullet, player.HP, player.Life, true);

            return true;
        }

        public bool GetPlayerRequest(HostID hostID, RmiContext rmiContext, string id, string tag)
        {
            List<PlayerDTO> players = DAO.GetPlayers(id);

            if (players.Count == 0 || players == null)
            {
                return false;
            }

            foreach (PlayerDTO player in players)
            {
                Proxy.GetPlayerResponse(hostID, rmiContext, player.UserID, player.Tag, player.Bullet, player.HP, player.Life, true);
            }

            return true;
        }
    }
}
