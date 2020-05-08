using System;
using MySql.Data.MySqlClient;

namespace MySQL_DotNetConnection
{
    class GameDTO // Data Transfer Object
    {
        public string ID { get; set; }
        public string Password { get; set; }
        public string Name { get; set; }
        public string Email { get; set; }

        public GameDTO(string id, string password, string name, string email)
        {
            ID = id;
            Password = password;
            Name = name;
            Email = email;
        }
    }

    class GameDAO // Data Access Object
    {
        private MySqlConnection conn;

        public GameDAO()
        {
            string connectionInfo = "Server=localhost;Port=3306;User=root;Database=GameDB;Password=1234;";
            conn = new MySqlConnection(connectionInfo);
        }

        public bool Register(GameDTO gameDTO)
        {
            bool bIsSuccess = false;
            conn.Open();

            string query = $"Insert Into GameUserTable (ID, Password, Name, Email) Values ('{gameDTO.ID}', '{gameDTO.Password}', '{gameDTO.Name}', '{gameDTO.Email}')";
            MySqlCommand command = new MySqlCommand(query, conn);
            int result = command.ExecuteNonQuery();
            if (result == 1)
            {
                Console.WriteLine("Insert Successful!");
                bIsSuccess = true;
            }
            else
            {
                Console.WriteLine("Insert Error!");
            }

            conn.Close();
            return bIsSuccess;
        }

        public bool GetInfo(string userID)
        {
            bool bIsSuccessful = false;
            conn.Open();

            string query = $"Select * From GameUserTable Where ID = '{userID}'";
            MySqlCommand command = new MySqlCommand(query, conn);
            MySqlDataReader reader = command.ExecuteReader();

            if (reader.Read())
            {
                Console.WriteLine($"ID : {reader["ID"]}");
                Console.WriteLine($"Name : {reader["Name"]}");
                Console.WriteLine($"Email : {reader["Email"]}");
                Console.WriteLine("==================================");
                bIsSuccessful = true;
            }
            else
            {
                Console.WriteLine("GetInfo() Error!");
            }

            conn.Close();
            return bIsSuccessful;
        }

        public bool Login(GameDTO gameDTO)
        {
            conn.Open();

            string query = $"Select * From GameUserTable Where ID = '{gameDTO.ID}' And Password = '{gameDTO.Password}'";
            MySqlCommand mySqlCommand = new MySqlCommand(query, conn);
            MySqlDataReader mySqlDataReader = mySqlCommand.ExecuteReader();

            if (mySqlDataReader.Read())
            {
                conn.Close();
                return true;
            }
            else
            {
                conn.Close();
                return false;
            }
        }
    }

    class Program
    {
        public enum Menu
        {
            Register,
            Login,
            GetInfo,
            Exit
        }

        static void Main(string[] args)
        {
            GameDAO gamedbDAO = new GameDAO();

            Console.WriteLine($"1. Register 2. Login 3. GetInfo 4. Exit");
            Menu select = (Menu)(int.Parse(Console.ReadLine()) - 1);
            
            switch (select)
            {
                case Menu.Register:
                    {
                        Console.WriteLine($"============ Register ============");
                        Console.Write($"ID : "); string id = Console.ReadLine();
                        Console.Write($"PW : "); string pw = Console.ReadLine();
                        Console.Write($"Name : "); string name = Console.ReadLine();
                        Console.Write($"Email : "); string email = Console.ReadLine();
                        Console.WriteLine($"==================================");

                        if (gamedbDAO.Register(new GameDTO(id, pw, name, email)))
                        {
                            Console.WriteLine($"Register Successful!");
                        }
                        else
                        {
                            Console.WriteLine($"Register Failed.");
                        }

                        break;
                    }

                case Menu.Login:
                    {
                        Console.WriteLine($"============ Login ============");
                        Console.Write($"ID : "); string id = Console.ReadLine();
                        Console.Write($"Password : "); string pw = Console.ReadLine();
                        Console.WriteLine($"===============================");

                        if (gamedbDAO.Login(new GameDTO(id, pw, null, null)))
                        {
                            Console.WriteLine($"Login Successful!");
                        }
                        else
                        {
                            Console.WriteLine($"Login Failed.");
                        }

                        break;
                    }

                case Menu.GetInfo:
                    {
                        Console.WriteLine($"============ Get Info ============");
                        Console.Write($"ID : "); string id = Console.ReadLine();
                        Console.WriteLine($"===============================");

                        if (gamedbDAO.GetInfo(id))
                        {
                            Console.WriteLine("Get Complete");
                        }
                        else
                        {
                            Console.WriteLine("Get Failed");
                        }

                        break;
                    }

                case Menu.Exit:
                    {
                        
                        break;
                    }
            }
        }
    }
}
