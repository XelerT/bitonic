#pragma once

#include <cassert>
#include "CL/opencl.hpp"

namespace opencl 
{
        struct config_t
        {
                size_t data_sz   = 1;   // number of 256-blocks
                size_t global_sz = 1;   // number of 256-datagroups
                size_t local_sz  = 1;
                
                const char *kernel_file_name = "bitonic_sort.cl";
                cl::QueueProperties q_props = cl::QueueProperties::Profiling | 
                                              cl::QueueProperties::OutOfOrder;
                inline config_t read_cfg (int argc, char **argv);
                // void dump(std::ostream &Os);
        };

        // static std::ostream &operator<<(std::ostream &Os, Config Cfg)
        // {
        //         Cfg.dump(Os);
        //         return Os;
        // }

        class opencl_app_t
        {
                protected:
                        cl::Platform     platform;
                        cl::Context      context;
                        cl::CommandQueue queue;
                        std::string      kernel;
                        config_t         config;

                        inline cl::Platform select_platform ();
                        inline cl::Context  get_gpu_context(cl_platform_id platform_id);

                public:
                        opencl_app_t (config_t &config_):
                                platform(select_platform()),     context(get_gpu_context(platform())), 
                                queue(context, config_.q_props), kernel(read_file(config_.kernel_file_name)), 
                                config(config_) {}
                                // cl::string name    = P_.getInfo<CL_PLATFORM_NAME>();
                                // cl::string profile = P_.getInfo<CL_PLATFORM_PROFILE>();       

                        size_t local_sz() const { return config.local_sz; }
        };

        template <typename T>
        class bitonic_sort_t final : public opencl_app_t
        {
                using opencl_app_t::platform;
                using opencl_app_t::context;
                using opencl_app_t::queue;
                using opencl_app_t::kernel;
                using opencl_app_t::config;

                public:
                        using bitonic_sort_kernel = cl::KernelFunctor<cl::Buffer, cl_int>;

                        bitonic_sort_t (config_t &config_): 
                                opencl_app_t(config_) {}

                        cl::Event sort (T *data, T *sorted_data, size_t n_elems);
        };

        cl::Platform opencl_app_t::select_platform ()
        {
                cl::vector<cl::Platform> platforms;
                cl::Platform::get(&platforms);

                for (auto p : platforms) {
                        cl_uint n_devices = 0;
                        ::clGetDeviceIDs(p(), CL_DEVICE_TYPE_GPU, 0, NULL, &n_devices);
                        if (n_devices > 0)
                                return cl::Platform(p);
                }
                throw std::runtime_error("No platform selected");
        }

        cl::Context opencl_app_t::get_gpu_context (cl_platform_id platform_id) 
        {
                cl_context_properties properties[] = 
                {
                        CL_CONTEXT_PLATFORM, 
                        reinterpret_cast<cl_context_properties>(platform_id), 
                        0 // end of properties
                };

                return cl::Context(CL_DEVICE_TYPE_GPU, properties);
        }

        template <typename T>
        cl::Event bitonic_sort_t<T>::sort (T *data, T *sorted_data, size_t n_elems) 
        {
                assert(data);
                assert(sorted_data);

                size_t data_size = n_elems * sizeof(T);

                cl::Buffer cl_data {context, CL_MEM_READ_WRITE, data_size};
                cl::copy(queue, data, data + n_elems, cl_data);

                cl::Program program {context, kernel, true /* build immediately */};

                bitonic_sort_kernel bitonic_sort(program, "bitonic_sort");

                cl::NDRange gl_range {config.global_sz};
                cl::NDRange lc_range {local_sz()};
                cl::EnqueueArgs args {queue, gl_range, lc_range};
                
                cl::Event event = bitonic_sort(args, cl_data, n_elems);
                event.wait();

                cl::copy(queue, cl_data, sorted_data, sorted_data + n_elems);
                return event; // to collect profiling info
        }
}
