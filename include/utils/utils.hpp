#pragma once

#include <iostream>
#include <random>
#include <fstream>

enum class status_t
{
        OK,
        ERROR,
        WARNING,
        DEGENERATED
};

// std::ostream& operator<<(std::ostream &os, const status_t &stat)
// {
//         os << static_cast<std::underlying_type<status_t>::type>(stat);
//         return os;
// }

const double THRESHOLD = 1e-5;
const size_t MAX_SIZE_T_VALUE = static_cast<size_t>(-1);

template <typename T> void construct (T *data, const T &rhs) { new (data) T(rhs); }
template <typename T> void construct (T *data, T &&rhs)
{
        new (data) T(std::move(rhs));
}

template <class T> void destroy (T *data) { data->~T(); }
template <typename It> void destroy (It begin, It end)
{
        while (begin != end) {
                destroy(begin);
                begin++;
        }
}

template <typename It> void rand_init (It start, It end, int low, int up)
{
        static std::mt19937_64 mt_source;
        std::uniform_int_distribution<int> dist(low, up);

        for (It cur = start; cur != end; ++cur)
                *cur = dist(mt_source);
}

std::string read_file (const std::string &file_path);
