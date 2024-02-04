#pragma once

#include <vector>
#include <iterator>

template <typename T>
void print (const std::vector<T> &vec)
{
        for (auto it = vec.cbegin(); it != vec.cend(); ++it) {
                std::cout << *it << " ";
                std::cout.flush();
        }

        // std::cout << std::endl;
}

template <typename T>
void print (const std::vector<T> &vec, size_t size)
{
        for (size_t i = 0; i < size; i++)
                std::cout << vec[i] << " ";

        // std::cout << std::endl;
}

template <typename T>
void println (const std::vector<T> &vec)
{
        for (auto it = vec.cbegin(); it != vec.cend(); it++)
                std::cout << *it << "\n";
}