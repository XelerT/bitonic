#pragma once

#include <vector>
#include <utility>

// #include "matrixes/chain.hpp"

std::pair<size_t, std::vector<double>>
get_user_data ();

std::pair<size_t, std::vector<size_t>>
get_user_chain_data ();

// std::vector<int> get_matrixes ();
// matrixes::matrix_chain_t<int> create_matrix_chain (std::vector<int> &sizes_elems);