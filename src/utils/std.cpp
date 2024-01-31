#include "std.hpp"

int length (int number, int base)
{
        int len = 0;

        if (number < 0) {
                len++;
                number *= -1;
        }
        while (number > 0) {
                number /= base;
                len++;
        }

        return len;
}