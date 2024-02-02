#include <iostream>
#include <chrono>
#include <stdexcept>
#include <cmath>

#ifndef CL_HPP_TARGET_OPENCL_VERSION
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#endif

#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_ENABLE_EXCEPTIONS

#include "utils.hpp"
#include "std_vector.hpp"
#include "ui.hpp"
#include "opencl.hpp"
#include "bitonic.hpp"

using namespace std;
using namespace opencl;
using namespace bitonic;

#define dbgs                                                                   \
  if (!ANALYZE) {                                                              \
  } else                                                                       \
    std::cout

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

struct option_error : public std::runtime_error {
  option_error(const char *s) : std::runtime_error(s) {}
};

int main () try
{
        std::chrono::high_resolution_clock::time_point t_start, t_fin;
        
        // config_t config {16'384, 32'768, 1, "../include/opencl/bitonic_sort.cl"};
        // config_t config {32'768, 32'768, 1, "../include/opencl/bitonic_sort.cl"};
        // config_t config {64'536, 32'768, 1, "../include/opencl/bitonic_sort.cl"};
        // config_t config {262'144, 32'768, 1, "../include/opencl/bitonic_sort.cl"};
        config_t config {1'048'576, 1'048'576, 1, "../include/opencl/bitonic_sort.cl"};
        bitonic_sort_t<int> app(config);

        cl::vector<int> data(config.data_sz);
        cl::vector<int> sorted_data(data.size());

        rand_init(data.begin(), data.end(), 0, 0xF);

        t_start = std::chrono::high_resolution_clock::now();
        cl_long GPU_duration = app.sort(data.data(), sorted_data.data(), data.size());
        t_fin = std::chrono::high_resolution_clock::now();

        long duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(t_fin - t_start).count();
        
        std::cout << "GPU wall time measured: " << duration << " ms" << std::endl;
        std::cout << "GPU pure time measured: " << GPU_duration / 1000000 << " ms" << std::endl;

        t_start = std::chrono::high_resolution_clock::now();
        std::sort(data.begin(), data.end());
        t_fin = std::chrono::high_resolution_clock::now();
        duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(t_fin - t_start).count();
        std::cout << "std::sort time measured: " << duration << " ms" << std::endl;
        
        if (data == sorted_data)
                std::cout << "Passed.\n";
        else {
                for (size_t i = 0, cnt = 0; i < data.size() && cnt < 16; i++)
                        if (data[i] != sorted_data[i]) {
                                std::cout << i << " " << data[i] << " " << sorted_data[i] << "\n"; 
                                cnt++;
                        }
                std::cout << "Not passed.\n";
        }

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