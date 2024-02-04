#include <iostream>
#include <chrono>
#include <stdexcept>
#include <cmath>

#include "utils.hpp"
#include "std_vector.hpp"
#include "ui.hpp"
#include "opencl.hpp"
#include "bitonic.hpp"

#include "cmd_parser.hpp"

using namespace std;
using namespace opencl;
using namespace bitonic;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

struct option_error : public std::runtime_error {
        option_error(const char *s) : std::runtime_error(s) {}
};

int main (int n_args, const char *args[]) try
{
        arguments_t parsed_args = parse_cmd_args(n_args, args);
        if (!parsed_args.parsed)
                return 0;

        cl::vector<int> data = get_user_elems();
        config_t config      = create_bitonic_config(parsed_args, data.size());
                
        bitonic_sort_t<int> app(config);

        cl::vector<int> sorted_data(data.size());
        cl::vector<int> data2(data);

        std::chrono::high_resolution_clock::time_point t_start, t_fin;
        t_start = std::chrono::high_resolution_clock::now();
        cl_long GPU_duration = app.sort(data.data(), sorted_data.data(), data.size());
        t_fin = std::chrono::high_resolution_clock::now();

        long duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(t_fin - t_start).count();
        
        if (parsed_args.time || parsed_args.comp_with != "") {
                std::cout << "GPU wall time measured: " << duration << " ms" << std::endl;
                std::cout << "GPU pure time measured: " << GPU_duration / 1000000 << " ms" << std::endl;
        }

        if (parsed_args.comp_with == "sort") {
                t_start = std::chrono::high_resolution_clock::now();
                std::sort(data.begin(), data.end());
                t_fin = std::chrono::high_resolution_clock::now();
                duration =
                std::chrono::duration_cast<std::chrono::milliseconds>(t_fin - t_start).count();
                std::cout << "std::sort time measured: " << duration << " ms" << std::endl;
        }

        if (parsed_args.comp_with == "cpu") {
                t_start = std::chrono::high_resolution_clock::now();
                sort(data2.data(), data2.size());
                t_fin = std::chrono::high_resolution_clock::now();
                duration =
                std::chrono::duration_cast<std::chrono::milliseconds>(t_fin - t_start).count();
                std::cout << "cpu bitonic sort time measured: " << duration << " ms" << std::endl;
        }

        if (parsed_args.print_sorted_data)
                print(sorted_data);

        return 0;

} catch (cl::BuildError &err) {
        std::cerr << "OCL BUILD ERROR: " << err.err() << ":" << err.what()
                  << std::endl;
        std::cerr << "-- Log --\n";
        for (auto e : err.getBuildLog())
                std::cerr << e.second;
        std::cerr << "-- End log --\n";
        return -1;
} catch (cl::Error &err) {
        std::cerr << "OCL ERROR: " << err.err() << ":" << err.what() << std::endl;
        return -1;
} catch (option_error &err) {
        std::cerr << "INVALID OPTION: " << err.what() << std::endl;
        return -1;
} catch (std::runtime_error &err) {
        std::cerr << "RUNTIME ERROR: " << err.what() << std::endl;
        return -1;
} catch (...) {
        std::cerr << "UNKNOWN ERROR\n";
        return -1;
}