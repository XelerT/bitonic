#pragma once

#include <vector>
#include <iterator>

template <typename T>
void print (const std::vector<T> &vec)
{
        for (auto it = vec.cbegin(); it != vec.cend(); it++)
                std::cout << *it << " ";

        // std::cout << std::endl;
}

template <typename T>
void println (const std::vector<T> &vec)
{
        for (auto it = vec.cbegin(); it != vec.cend(); it++)
                std::cout << *it << "\n";
}