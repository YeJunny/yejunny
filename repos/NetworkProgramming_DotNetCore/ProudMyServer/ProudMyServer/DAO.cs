using MySql.Data.MySqlClient;
using System;
using System.Collections.Generic;

namespace ProudMyServer
{
    class DAO
    {
        private MySqlConnection conn;

        public DAO()
        {
            string connectionInfo = "Server=127.0.0.1;Port=3306;User=root;Database=GameDB;Password=1234;";
            conn = new MySqlConnection(connectionInfo);
        }

        public PlayerDTO CreatePlayer(PlayerDTO player)
        {
            try
            {
                conn.Open();

                string sql = $"Insert Into player_db (userid, tag) Values ('{player.UserID}', '{player.Tag}')";
                MySqlCommand command = new MySqlCommand(sql, conn);
                int result = command.ExecuteNonQuery();

                conn.Close();
                
                PlayerDTO fullPlayer = null;
                if (result == 1)
                {
                    fullPlayer = GetPlayer(player.UserID, player.Tag);
                }
                else
                {
                    Console.WriteLine("CreatePlayer() Insert - Failed");
                }
                
                return fullPlayer;
            }
            catch (MySqlException e)
            {
                Console.WriteLine($"Create Player - Failed : {e}");
                return null;
            }
        }

        public bool Register(AccountDTO accountDTO)
        {
            try
            {
                bool bIsSuccess = false;
                conn.Open();

                string query = $"Insert Into account_db (userid, userpw, username) Values ('{accountDTO.UserID}', '{accountDTO.UserPW}', '{accountDTO.UserName}')";
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
            catch (MySqlException e)
            {
                Console.WriteLine($"Loing Exeption : {e}");
                conn.Close();
                return false;
            }
        }

        public PlayerDTO GetPlayer(string id, string tag)
        {
            try
            {
                conn.Open();

                string sql = $"Select * From player_db Where userid = '{id}' And tag = '{tag}'";
                MySqlCommand command = new MySqlCommand(sql, conn);
                MySqlDataReader reader = command.ExecuteReader();

                PlayerDTO player = null;

                if (reader.Read())
                {
                    player = new PlayerDTO(
                        reader["userid"].ToString(),
                        reader["tag"].ToString(),
                        int.Parse(reader["bullet"].ToString()),
                        int.Parse(reader["hp"].ToString()),
                        int.Parse(reader["life"].ToString())
                    );
                }
                else
                {
                    Console.WriteLine("Get Player() - Failed");
                }

                conn.Close();
                return player;
            }
            catch (MySqlException e)
            {
                Console.WriteLine($"Get Player - Failed : {e}");
                return null;
            }
        }

        public List<PlayerDTO> GetPlayers(string userID)
        {
            try
            {
                conn.Open();

                List<PlayerDTO> players = new List<PlayerDTO>();

                string query = $"Select * From player_db Where userid = '{userID}'";
                MySqlCommand command = new MySqlCommand(query, conn);
                MySqlDataReader reader = command.ExecuteReader();

                while (reader.Read())
                {
                    PlayerDTO player = new PlayerDTO(
                        reader["userid"].ToString(),
                        reader["tag"].ToString(),
                        int.Parse(reader["bullet"].ToString()),
                        int.Parse(reader["hp"].ToString()),
                        int.Parse(reader["life"].ToString())
                    );
                    players.Add(player);
                }

                conn.Close();
                return players;
            }
            catch (MySqlException e)
            {
                Console.WriteLine($"Get Players - Failed : {e}");
                return null;
            }
        }

        public bool Login(AccountDTO accountDTO)
        {
            try
            {
                conn.Open();

                string query = $"Select * From account_db Where userid = '{accountDTO.UserID}' And userpw = '{accountDTO.UserPW}'";
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
            catch (MySqlException e)
            {
                Console.WriteLine($"Loing Exeption : {e}");
                conn.Close();
                return false;
            }
        }
    }
}
