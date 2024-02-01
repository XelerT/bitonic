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

                size_t n_stages  = global_sz / local_sz;
                
                const char *kernel_file_name = "bitonic_sort.cl";
                cl::QueueProperties q_props = cl::QueueProperties::Profiling | 
                                              cl::QueueProperties::OutOfOrder;

                config_t () = default;
                config_t (size_t data_sz_,  size_t global_sz_,
                          size_t local_sz_, size_t n_stages_, const char *kernel_file_name_):
                          data_sz(data_sz_), global_sz(global_sz_), 
                          local_sz(local_sz_), n_stages(n_stages_),
                          kernel_file_name(kernel_file_name_) {}
                config_t (size_t data_sz_,  size_t global_sz_,
                          size_t local_sz_, const char *kernel_file_name_):
                          data_sz(data_sz_), global_sz(global_sz_), 
                          local_sz(local_sz_),
                          kernel_file_name(kernel_file_name_) {}
        };

        class opencl_app_t
        {
                protected:
                        cl::Platform     platform;
                        cl::Context      context;
                        cl::CommandQueue queue;
                        std::string      kernel;
                        config_t         config;

                        inline cl::Platform select_platform ();
                        inline cl::Context  get_gpu_context (cl_platform_id platform_id);

                public:
                        opencl_app_t (config_t &config_):
                                platform(select_platform()),     context(get_gpu_context(platform())), 
                                queue(context, config_.q_props), kernel(read_file(config_.kernel_file_name)), 
                                config(config_) {}
                                // cl::string name    = P_.getInfo<CL_PLATFORM_NAME>();
                                // cl::string profile = P_.getInfo<CL_PLATFORM_PROFILE>();       

                        size_t local_sz() const { return config.local_sz; }

                        ~opencl_app_t () {};
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
                        using sort_init_kernel       = cl::KernelFunctor<cl::Buffer>;
                        using sort_stage_n_kernel    = cl::KernelFunctor<cl::Buffer, cl_int>;
                        using sort_stage_0_kernel    = cl::KernelFunctor<cl::Buffer, cl_int>;
                        using sort_merge_kernel      = cl::KernelFunctor<cl::Buffer, cl_int>;
                        using sort_merge_last_kernel = cl::KernelFunctor<cl::Buffer>;

                        bitonic_sort_t (config_t &config_): 
                                opencl_app_t(config_) {}

                        cl_long sort (T *data, T *sorted_data, size_t n_elems);
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
        cl_long bitonic_sort_t<T>::sort (T *data, T *sorted_data, size_t n_elems) 
        {
                assert(data);
                assert(sorted_data);

                size_t data_size = n_elems * sizeof(T);

                cl::Buffer cl_data {context, CL_MEM_READ_WRITE, data_size};
                cl::copy(queue, data, data + n_elems, cl_data);

                cl::Program program {context, kernel, true /* build immediately */};

                sort_init_kernel       sort_init       {program, "bitonic_sort_init"};
                sort_stage_n_kernel    sort_stage_n    {program, "bitonic_sort_stage_n"};
                sort_stage_0_kernel    sort_stage_0    {program, "bitonic_sort_stage_0"};
                sort_merge_kernel      sort_merge      {program, "bitonic_sort_merge"};
                sort_merge_last_kernel sort_merge_last {program, "bitonic_sort_merge_last"};
                
                cl::NDRange gl_range {config.global_sz};
                cl::NDRange lc_range {local_sz()};
                cl::EnqueueArgs args {queue, gl_range, lc_range};
                
                cl::Event event1 = sort_init(args, cl_data);
                event1.wait();
                cl_ulong GPU_t_start = event1.getProfilingInfo<CL_PROFILING_COMMAND_START>();
                cl_ulong GPU_t_fin   = event1.getProfilingInfo<CL_PROFILING_COMMAND_END>();
                cl_ulong GPU_duration = GPU_t_fin - GPU_t_start;

                // for (int k = 2; k < config.n_stages; k *= 2) {
                //         for (int j = k; j > 0; j /= 2) {
                //                 cl::Event event2 = sort_stage_n(args, cl_data, j);
                //                 event2.wait();
                //                 GPU_t_start = event2.getProfilingInfo<CL_PROFILING_COMMAND_START>();
                //                 GPU_t_fin   = event2.getProfilingInfo<CL_PROFILING_COMMAND_END>();
                //                 GPU_duration += GPU_t_fin - GPU_t_start;
                //         }
                //         cl::Event event2 = sort_stage_0(args, cl_data, k);
                //         event2.wait();
                //         GPU_t_start = event2.getProfilingInfo<CL_PROFILING_COMMAND_START>();
                //         GPU_t_fin   = event2.getProfilingInfo<CL_PROFILING_COMMAND_END>();
                //         GPU_duration += GPU_t_fin - GPU_t_start;
                // }

                // for (int j = config.n_stages; j > 0; j /= 2) {
                //         cl::Event event2 = sort_merge(args, cl_data, j);
                //         event2.wait();
                //         GPU_t_start = event2.getProfilingInfo<CL_PROFILING_COMMAND_START>();
                //         GPU_t_fin   = event2.getProfilingInfo<CL_PROFILING_COMMAND_END>();
                //         GPU_duration += GPU_t_fin - GPU_t_start;
                // }

                // cl::Event event2 = sort_merge_last(args, cl_data);
                // event2.wait();
                // GPU_t_start = event2.getProfilingInfo<CL_PROFILING_COMMAND_START>();
                // GPU_t_fin   = event2.getProfilingInfo<CL_PROFILING_COMMAND_END>();
                // GPU_duration += GPU_t_fin - GPU_t_start;

                cl::copy(queue, cl_data, sorted_data, sorted_data + n_elems);
                return GPU_duration; // to collect profiling info
        }
}
