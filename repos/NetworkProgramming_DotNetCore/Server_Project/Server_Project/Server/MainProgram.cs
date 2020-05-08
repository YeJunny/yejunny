using System;
using System.Collections.Generic;
using System.Text;

namespace Server_Project
{
    class MainProgram
    {
        public enum EMenu
        {
            PrintList,
            Exit
        }

        public static void Main(string[] args)
        {
            Console.Write("Room Name : ");
            string roomName = Console.ReadLine();

            ChatingServer server = new ChatingServer(5000, 5, roomName);

            while (true)
            {
                Console.WriteLine("==========================================");
                Console.WriteLine("1. Print All User List / 2. Exit");
                Console.Write("Select : ");
                EMenu select = (EMenu)(int.Parse(Console.ReadLine()) - 1);

                switch (select)
                {
                    case EMenu.PrintList:
                        {
                            server.PrintUserAll();
                            break;
                        }

                    case EMenu.Exit:
                        {
                            goto ExitLable;
                        }

                    default:
                        {
                            Console.WriteLine("Invaild Input. Retry Input...");
                            break;
                        }
                }
            }

        ExitLable:
            server.Close();
        }
    }
}
