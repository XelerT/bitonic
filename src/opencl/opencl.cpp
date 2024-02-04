#include "ui/cmd_parser.hpp"
#include "opencl.hpp"

using namespace opencl;

config_t opencl::create_bitonic_config (arguments_t &parsed_args, size_t n_data_elems)
{
        config_t config {};

        if (parsed_args.gl_size) { config.glob_sz  = parsed_args.gl_size; }
        else                      { config.glob_sz  = n_data_elems / 2; }
        if (parsed_args.lc_size) { config.local_sz = parsed_args.lc_size; }
        else                      { config.local_sz = 1; }
        if (parsed_args.offset != MAX_SIZE_T_VALUE) 
                                  { config.offset = parsed_args.offset; }
        else                      { config.offset = 0; }
        
        config.kernel_path = parsed_args.kernel_path;

        return config;
}
