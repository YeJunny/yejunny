using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Net;
using System.Text;
using MySql.Data.MySqlClient;

namespace DBServer
{
    [Serializable]
    class GameDTO
    {
        public string ID { get; set; }
        public string HashPassword { get; set; }
        public string Name { get; set; }
        public string Email { get; set; }

        public GameDTO(string id, string hashPassword, string name, string email)
        {
            ID = id;
            HashPassword = hashPassword;
            Name = name;
            Email = email;
        }
    }

    class GameDAO
    {
        private MySqlConnection con;

        public GameDAO()
        {
            string option = "Server=localhost;Port=3306;User=root;Password=1234;Database=GameDB";
            con = new MySqlConnection(option);
        }

        public bool Register(GameDTO user)
        {
            bool bIsSuccess = false;

            con.Open();

            string query = $"Insert Into GameDB (ID, Password, Name, Email) Values ('{user.ID}', '{user.HashPassword}', '{user.Name}', '{user.Email}')";

            MySqlCommand com = new MySqlCommand(query, con);
            if (com.ExecuteNonQuery() == 1)
            {
                bIsSuccess = true;  
            }

            con.Close();
            return bIsSuccess;
        }

        public GameDTO Login(string id, string hashPassword)
        {
            con.Open();

            string query = $"Select * From GameDB Where ID = '{id}' And Password = '{hashPassword}'";

            MySqlCommand com = new MySqlCommand(query, con);
            MySqlDataReader read = com.ExecuteReader();

            GameDTO user;

            if (read.Read())
            {
                user = new GameDTO((string)read["ID"], (string)read["Password"], (string)read["Name"], (string)read["Email"]);
            }
            else
            {
                user = null;
            }

            con.Close();
            return user;
        }

        public GameDTO GetUser(string id, string hashPassword)
        {
            con.Open();

            string query = $"Select * From GameDB Where ID = '{id}' And Password = '{hashPassword}'";

            MySqlCommand com = new MySqlCommand(query, con);
            MySqlDataReader read = com.ExecuteReader();

            GameDTO user;

            if (read.Read())
            {
                user = new GameDTO((string)read["ID"], (string)read["Password"], (string)read["Name"], (string)read["Email"]);
            }
            else
            {
                user = null;
            }

            con.Close();
            return user;
        }
    }
}
