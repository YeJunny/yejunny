using System;
using System.Collections.Generic;
using System.Text;

namespace ProudMyServer
{
    class AccountDTO
    {
        public string UserID { get; set; }
        public string UserPW { get; set; }
        public string UserName { get; set; }
        public string RegDate { get; set; }

        public AccountDTO(string id, string password, string name)
        {
            UserID = id;
            UserPW = password;
            UserName = name;
        }
    }
}
