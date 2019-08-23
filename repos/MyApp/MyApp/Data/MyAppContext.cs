using Microsoft.AspNetCore.Identity.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore;
using MyApp.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace MyApp.Data
{
    public class MyAppContext : DbContext
    {
        public MyAppContext(DbContextOptions options) : base(options) {}

        public DbSet<Student> Students { get; set; }
        public DbSet<Teacher> Teachers { get; set; }

        internal Task AddRangeAsync(List<Teacher> teachers)
        {
            throw new NotImplementedException();
        }

        internal Task SaveChangesAsync()
        {
            throw new NotImplementedException();
        }
    }
}
