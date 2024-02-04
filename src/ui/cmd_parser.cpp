#include <boost/program_options.hpp>
#include <iostream>

#include "cmd_parser.hpp"

namespace po = boost::program_options;

arguments_t parse_cmd_args (int n_args, const char **args)
{
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help,h",   "display help info and exit")
                ("globsz",   po::value<size_t>(),      "Set global size")
                ("locsz",    po::value<size_t>(),      "Set local size")
                ("kernel,k", po::value<std::string>(), "Set path to the kernel file")
                ("offset,o", po::value<size_t>(),      "Set kernel offset in queue")
                ("compare",  po::value<std::string>(), "Turn on comparison sorts: \n \
                                                                cpu  - bitonic sort using cpu,\n \
                                                                sort - std sort.")
                ("time",     "Print consumed time to sort data.")
                ("print",    po::value<bool>(), "Print sorted data (default value: true)")
                ;

        po::variables_map var_map {};
        po::store(po::parse_command_line(n_args, args, desc), var_map);
        po::notify(var_map);

        arguments_t parsed_args {};

        if (var_map.count("help")) { 
                std::cout << desc << std::endl; 
                parsed_args.parsed = false; 
                
                return parsed_args; 
        }

        if (var_map.count("globsz"))  { parsed_args.gl_size     = var_map["globsz"].as<size_t>(); }
        if (var_map.count("locsz"))   { parsed_args.lc_size     = var_map["locsz"].as<size_t>(); }
        if (var_map.count("kernel"))  { parsed_args.kernel_path = var_map["kernel"].as<std::string>(); }
        if (var_map.count("offset"))  { parsed_args.offset      = var_map["offset"].as<size_t>(); }
        if (var_map.count("compare")) { parsed_args.comp_with   = var_map["compare"].as<std::string>(); }
        if (var_map.count("time"))    { parsed_args.time = true; }
        if (var_map.count("print"))   { parsed_args.print_sorted_data = var_map["print"].as<bool>(); }

        return parsed_args;
}