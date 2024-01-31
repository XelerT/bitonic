#pragma once

#include <list>
#include <iostream>

int length (int number, int base = 10);

template <typename T>
void print (const std::list<T> &list)
{
        for (auto it : list)
                std::cout << it << " ";

        // std::cout << std::endl;
}

template <typename T>
void println (const std::list<T> &list)
{
        for (auto it : list)
                std::cout << it << "\n";
}