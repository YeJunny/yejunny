using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using MyApp.Models;
using MyApp.ViewModels;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace MyApp.Controllers
{
    public class HomeController : Controller
    {
        public IActionResult Index()
        {
            List<Teacher> teachers = new List<Teacher>()
            {
                new Teacher() { Name = "세종대왕", Class = "한글" },
                new Teacher() { Name = "이순신", Class = "해상전략" },
                new Teacher() { Name = "제갈량", Class = "지략" },
                new Teacher() { Name = "을지문덕", Class = "지상전략" }
            };

            var viewModel = new StudentTeacherViewModel()
            {
                Student = new Student(),
                Teachers = teachers
            };

            return View(viewModel);
        }

        public IActionResult Student()
        {
            var viewModel = new StudentTeacherViewModel()
            {
                Student = new Student(),
            };

            return View(viewModel);
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult Student(StudentTeacherViewModel model)
        {
            if (ModelState.IsValid)
            {
                ModelState.Clear();
            }
            else
            {
                // 에러를 보여준다
            }

            var viewModel = new StudentTeacherViewModel()
            {
                Student = new Student(),
            };

            return View(viewModel);
        }
    }
}
