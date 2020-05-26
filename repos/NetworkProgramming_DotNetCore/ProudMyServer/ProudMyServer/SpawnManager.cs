using Nettention.Proud;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace ProudMyServer
{
    class SpawnManager
    {
        private Thread mThread;
        private NetServer mServer;

        public SpawnManager(NetServer server)
        {
            mThread = new Thread(new ThreadStart(Run));
            mServer = server;
        }

        public void Start()
        {
            mThread.Start();
        }

        public void Run()
        {
            while (true)
            {
                try
                {
                    HostID[] hostIDs = mServer.GetClientHostIDs();

                    foreach (HostID hostID in hostIDs)
                    {
                        int id = 100;
                        byte[] buffer = BitConverter.GetBytes(id);

                        ByteArray byteArray = new ByteArray(buffer);
                        mServer.SendUserMessage(hostID, RmiContext.ReliableSend, byteArray);
                    }

                    Thread.Sleep(1000);
                }
                catch (Exception e)
                {
                    Console.WriteLine($"{e.Message}");
                }
            }
        }
    }
}
