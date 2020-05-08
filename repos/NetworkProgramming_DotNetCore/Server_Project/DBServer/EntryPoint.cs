using System;
using System.Collections.Generic;
using System.Text;

namespace DBServer
{
    class EntryPoint
    {
        public enum EMenu
        {
            CreateRoom,
            DeleteRoom,
            PrintAllUser,
            Shutdown
        }

        public static void Main(String[] args)
        {
            Server server = new Server(5, 5000);

            while (true)
            {
                Console.WriteLine("================================================");
                Console.WriteLine(" S E R V E R");
                Console.WriteLine("================================================");
                Console.WriteLine("1. Create Room / 2. Delete Room / 3. Print All User / 4. Shutdown");
                Console.WriteLine("================================================");
                Console.Write("Select : ");

                int select;
                int.TryParse(Console.ReadLine(), out select);

                switch ((EMenu)(select - 1))
                {
                    case EMenu.CreateRoom:
                        {
                            string roomName = Console.ReadLine();
                            server.CreateRoom(roomName);
                            Console.WriteLine("Successful for Create Room!");

                            break;
                        }

                    case EMenu.DeleteRoom:
                        {
                            string roomName = Console.ReadLine();
                            server.DeleteRoom(roomName);
                            Console.WriteLine("Successful for Delete Room!");

                            break;
                        }

                    case EMenu.PrintAllUser:
                        {
                            string roomName = Console.ReadLine();
                            server.PrintAllUserAtRoom(roomName);
                            break;
                        }

                    case EMenu.Shutdown:
                        {
                            goto ExitLoop;
                        }

                    default:
                        {
                            Console.WriteLine("Invaild Input. Retry Input...");
                            break;
                        }
                }
            }

        ExitLoop:
            Console.WriteLine("Shutdown Server");
        }
    }
}
