using System;
using MySql.Data.MySqlClient;

namespace MySQL_DotNetConnection
{
    class Program
    {
        static void Main(string[] args)
        {
            string connectionString = "Server=localhost;Port=3306;User=root;Database=GameDB;Password=1234;";
            MySqlConnection connector = new MySqlConnection(connectionString);

            try
            {
                connector.Open();
                Console.WriteLine($"Connection Successed!");

                string query = "Select * From GameUserTable";
                MySqlCommand command = new MySqlCommand(query, connector);
                MySqlDataReader reader = command.ExecuteReader();

                Console.WriteLine($"-------Read Successed--------");

                while (reader.Read())
                {
                    Console.WriteLine("{0, 10}, {1, 10}, {2, 10}, {3, 18}, {4, 10}",
                        reader["ID"],
                        reader["Password"],
                        reader["Name"],
                        reader["Email"],
                        reader["RegisterDate"]);
                }

                connector.Close();
            }
            catch (Exception e)
            {
                Console.WriteLine($"Connection Or Query Error! : {e.Message}");
            }

            try
            {
                connector.Open();

                DateTime dateTime = DateTime.Now;
                string dateTimeStr = dateTime.ToString("yyyy-MM-dd");
                string query = $"Insert Into GameUserTable (ID, Password, Name, Email, RegisterDate) Values ('AddedUser', '3213', 'No', 'query@gmail.com', '{dateTimeStr}')";
                MySqlCommand command2 = new MySqlCommand(query, connector);

                if (command2.ExecuteNonQuery() == 1)
                {
                    Console.WriteLine($"Insert Successed!");
                }
                else
                {
                    Console.WriteLine($"Fail to insert.");
                }

                Console.WriteLine($"After Insert into GameUserTable.");
                string query2 = "Select * From GameUserTable";
                MySqlCommand command3 = new MySqlCommand(query2, connector);
                MySqlDataReader reader3 = command3.ExecuteReader();

                while (reader3.Read())
                {
                    Console.WriteLine("{0, 10}, {1, 15}, {2, 15}, {3, 20}, {4, 10}",
                        reader3.GetString(0),
                        reader3.GetString(1),
                        reader3.GetString(2),
                        reader3.GetString(3),
                        reader3["RegisterDate"]);
                }

                connector.Close();
            }
            catch (Exception e)
            {
                Console.WriteLine($"Fail Connection Or Query : {e.Message}");
            }
        }
    }
}
