#pragma once

#include "utils/utils.hpp"

struct arguments_t
{
        bool parsed    = true;
        size_t gl_size = 0;
        size_t lc_size = 0;
        size_t offset  = MAX_SIZE_T_VALUE;

        std::string kernel_path {"../include/opencl/bitonic_sort.cl"};
        std::string comp_with   {};
        bool time = false;
        bool print_sorted_data = true;
};

arguments_t parse_cmd_args (int n_args, const char **args);