using System;
using System.Collections.Generic;

namespace CSProject
{
    class Program
    {
        static void PrintHanoi(uint number, char from, char to)
        {
            if (number == 1)
            {
                Console.WriteLine($"원판을 {from} to {to}");
                return;
            }

            PrintHanoi(number - 1, 'A', 'B');
            Console.WriteLine($"원판을 {from} to {to}");

            PrintHanoi(1, 'A', 'C');
            Console.WriteLine($"원판을 {from} to {to}");

            PrintHanoi(number - 1, 'B', 'C');
            Console.WriteLine($"원판을 {from} to {to}");

        }

        static void Main(string[] args)
        {
            //PrintHanoi(3, 'A', 'C');

            int[] dummy = { 10, 20, 30, 30 };
            List<int> list = new List<int>();
            list.AddRange(dummy);
            bool bResult = list.Contains(30);
            int index = list.LastIndexOf(30);
        }
    }
}
