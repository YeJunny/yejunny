using System;
using System.Collections.Generic;
using System.Text;

namespace ProudMyServer
{
    class PlayerDTO
    {
        public string UserID { get; set; }
        public string Tag { get; set; }
        public int Bullet { get; set; }
        public int HP { get; set; }
        public int Life { get; set; }

        public PlayerDTO(string id, string tag, int bullet, int hp, int life)
        {
            UserID = id;
            Tag = tag;
            Bullet = bullet;
            HP = hp;
            Life = life;
        }
    }
}
