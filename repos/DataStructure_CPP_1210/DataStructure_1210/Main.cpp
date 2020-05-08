#include <iostream>
using namespace std;

void print_binary(int number)
{
    if (number) {
        print_binary(number >> 1);
        putc((number & 1) ? '1' : '0', stdout);
    }
}

int main(int argc, char** argv)
{
    float real = 0b11111111111111111111111111111111;
    printf("%f\n", real);
	return 0;
}