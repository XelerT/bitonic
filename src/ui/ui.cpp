#include <iostream>
#include <chrono>

#include "ui.hpp"
#include "utils/std.hpp"

using namespace std;
using namespace std::chrono;

vector<int> get_user_elems ()
{
        size_t n_elems;
        cin >> n_elems;

        vector<int> elems {};
        int input;

        while (elems.size() != n_elems && std::cin >> input)
                elems.push_back(input);

        if (std::cin.fail() && !std::cin.eof())
                throw std::runtime_error("You need to enter digits.");

        return elems;
}

