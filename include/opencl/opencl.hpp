#pragma once

#include <cassert>

#ifndef CL_HPP_TARGET_OPENCL_VERSION
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#endif

#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_ENABLE_EXCEPTIONS

#include "utils/utils.hpp"
#include "ui/cmd_parser.hpp"
#include "CL/opencl.hpp"

namespace opencl 
{
        struct config_t
        {
                size_t data_sz   = 1;
                size_t glob_sz   = 1;
                size_t local_sz  = 1;
                size_t offset    = 0;

                int n_stages = static_cast<int>(glob_sz / local_sz);
                
                std::string kernel_path = "bitonic_sort.cl";
                cl::QueueProperties q_props = cl::QueueProperties::Profiling | 
                                              cl::QueueProperties::OutOfOrder;

                config_t () = default;
                config_t (size_t data_sz_,  size_t glob_sz_,
                          size_t local_sz_, int    n_stages_, const char *kernel_path_):
                          data_sz(data_sz_), glob_sz(glob_sz_), 
                          local_sz(local_sz_), n_stages(n_stages_),
                          kernel_path(kernel_path_) {}
                config_t (size_t data_sz_,  size_t glob_sz_,
                          size_t local_sz_, const char *kernel_path_):
                          data_sz(data_sz_), glob_sz(glob_sz_), 
                          local_sz(local_sz_),
                          kernel_path(kernel_path_) {}
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
                                queue(context, config_.q_props), kernel(read_file(config_.kernel_path)), 
                                config(config_) { }
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
                        using sort_init_kernel       = cl::KernelFunctor<cl::Buffer, cl::LocalSpaceArg>;
                        using sort_stage_n_kernel    = cl::KernelFunctor<cl::Buffer, cl_uint, cl_uint>;
                        using sort_stage_0_kernel    = cl::KernelFunctor<cl::Buffer, cl::LocalSpaceArg, cl_int>;
                        using sort_merge_kernel      = cl::KernelFunctor<cl::Buffer, cl_uint>;
                        using sort_merge_last_kernel = cl::KernelFunctor<cl::Buffer, cl::LocalSpaceArg>;

                        bitonic_sort_t (config_t &config_): 
                                opencl_app_t(config_) {}

                        cl_long sort (T *data, T *sorted_data, size_t n_elems);
        };

//===================================================~~~FUNCTIONS~~~====================================================================

        config_t create_bitonic_config (arguments_t &parsed_args, size_t n_data_elems);


//===================================================~~~DECLARATIONS~~~====================================================================

//---------------------------------------------------~~~~~~Protected~~~~~~--------------------------------------------------------------------

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

//---------------------------------------------------~~~~~~Public~~~~~~--------------------------------------------------------------------

        template <typename T>
        cl_long bitonic_sort_t<T>::sort (T *data, T *sorted_data, size_t n_elems) 
        {
                assert(data);
                assert(sorted_data);

                size_t data_size = n_elems * sizeof(T);

                cl::Buffer cl_data {context, CL_MEM_READ_WRITE, data_size};
                cl::copy(queue, data, data + n_elems, cl_data);

                cl::Program program {context, kernel, true /* build immediately */};

                if (config.local_sz >= config.glob_sz)
                        config.glob_sz = config.local_sz;
                
                cl::Kernel sort_stage_n {program, "bitonic_sort_stage_n"};
                sort_stage_n.setArg(0, cl_data);

                cl::Event empty_event {};
                std::vector<cl::Event> empty_events {};
                queue.enqueueMarkerWithWaitList(&empty_events, &empty_event);
                empty_event.wait();
                cl_ulong GPU_t_start = empty_event.getProfilingInfo<CL_PROFILING_COMMAND_END>();

                cl::Event event {};
                for (int k = 2; k <= n_elems; k *= 2) {
                        sort_stage_n.setArg(1, k);
                        for (int j = k / 2; j > 0; j = j / 2) {
                                sort_stage_n.setArg(2, j);
                                queue.enqueueNDRangeKernel(sort_stage_n,   config.offset, 
                                                           config.glob_sz, config.local_sz, 
                                                           nullptr, &event);
                        }
                }
                event.wait();

                queue.enqueueMarkerWithWaitList(&empty_events, &empty_event);
                empty_event.wait();
                cl_ulong GPU_t_fin = empty_event.getProfilingInfo<CL_PROFILING_COMMAND_START>();

                cl::copy(queue, cl_data, sorted_data, sorted_data + n_elems);
                return GPU_t_fin - GPU_t_start;
        }
}
