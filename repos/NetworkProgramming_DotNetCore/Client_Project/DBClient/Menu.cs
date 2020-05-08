using System;
using System.Collections.Generic;
using System.Security.Cryptography;
using System.Text;

namespace DBClient
{
    class Menu
    {
        public enum EMenu
        {
            Reg,
            Login,
            GetInfo,
            Exit
        }

        public static void Main(string[] args)
        {
            Console.WriteLine("===========================================");
            Console.WriteLine("C H A T I N G");
            Console.WriteLine("===========================================");

            DBClient clinet = new DBClient();

            while (true)
            {
                Console.WriteLine("1. Register / 2. Login / 3. Exit");
                Console.WriteLine("===========================================");
                Console.Write("Select : ");
                EMenu menu = (EMenu)(int.Parse(Console.ReadLine()) - 1);

                switch (menu)
                {
                    case EMenu.Reg:
                        {
                            Console.Write("ID : "); string id = Console.ReadLine();
                            Console.Write("Password : "); string password = Console.ReadLine();
                            Console.Write("Name : "); string name = Console.ReadLine();
                            Console.Write("Email : "); string email = Console.ReadLine();
                            if (clinet.Register(id, password, name, email))
                            {
                                Console.WriteLine("Register Successful!");
                            }
                            else
                            {
                                Console.WriteLine("Register Failed...");
                            }
                            break;
                        }

                    case EMenu.Login:
                        {
                            Console.Write("ID : "); string id = Console.ReadLine();
                            Console.Write("Password : "); string password = Console.ReadLine();
                            Console.Write("Room Number : "); int roomNum = int.Parse(Console.ReadLine());
                            if (clinet.Login(id, password, roomNum))
                            {
                                Console.WriteLine("Login Successful!");
                                goto ChatStart;
                            }
                            else
                            {
                                Console.WriteLine("Login Failed...");
                                break;
                            }
                        }

                    case EMenu.Exit:
                        {
                            goto EndLable;
                        }

                    default:
                        {
                            Console.WriteLine("Invalid Input. Retry Input.");
                            break;
                        }
                }
            }

        ChatStart:
            while (true)
            {

            }

        EndLable:
            Console.WriteLine("Client End!");
        }
    }
}
