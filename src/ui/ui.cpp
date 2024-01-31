#include <iostream>
#include <chrono>

#include "ui.hpp"
#include "utils/std.hpp"

using namespace std;
using namespace std::chrono;

pair<size_t, vector<double>>
get_user_data ()
{
        size_t n_dimensions;
        cin >> n_dimensions;
        size_t n_elems = n_dimensions * n_dimensions;

        vector<double> elems {};
        double input;

        while (elems.size() != n_elems && std::cin >> input)
                elems.push_back(input);

        if (std::cin.fail() && !std::cin.eof())
                throw std::runtime_error("You need to enter digits.");

        return {n_dimensions, elems};
}

pair<size_t, vector<size_t>>
get_user_chain_data ()
{
        size_t n_matrixes;
        cin >> n_matrixes;

        vector<size_t> sizes {};
        size_t input;

        while (sizes.size() != n_matrixes && std::cin >> input)
                sizes.push_back(input);

        if (std::cin.fail() && !std::cin.eof())
                throw std::runtime_error("You need to enter digits.");

        return {n_matrixes, sizes};
}

vector<int> get_matrixes ()
{
        size_t n_data;
        cin >> n_data;

        vector<int> sizes_elems {};
        int input;

        while (sizes_elems.size() != n_data && std::cin >> input)
                sizes_elems.push_back(input);

        if (std::cin.fail() && !std::cin.eof())
                throw std::runtime_error("You need to enter digits.");

        return sizes_elems;
}

// matrix_chain_t<int> create_matrix_chain (vector<int> &sizes_elems)
// {
//         matrix_chain_t<int> chain {};

//         for (auto it = sizes_elems.begin(); it != sizes_elems.end();) {
//                 int n_rows = *it++;
//                 int n_cols = *it++;
//                 int n_matrix_elems = n_rows * n_cols;

//                 chain.add_matrix(new matrix_t<int> (n_rows, n_cols, it));
//                 it += n_matrix_elems;
//         }

//         return chain;
// }