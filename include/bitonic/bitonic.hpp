#pragma once

namespace bitonic
{
        template <typename T>
        void sort_req (T *data, size_t n_data, bool is_ascending)
        {
                assert(data);

                if (n_data > 1) {
                        size_t half_n_data = n_data / 2;
                        sort(data, half_n_data, true);
                        sort(data + half_n_data + 1, n_data - half_n_data, false);

                        merge(data, n_data, is_ascending);
                }
        }

        template <typename T>
        void merge_req (T *data, size_t n_data, bool is_ascending)
        {
                assert(data);

                size_t half_n_data = n_data / 2;
                if (n_data > 1) {
                        for (size_t i = 0; i < half_n_data; i++) {
                                if (data[i] > data[i + half_n_data] && is_ascending)
                                        std::swap(data[i], data[i + half_n_data]);
                        }
                        merge(data, half_n_data, is_ascending);
                        merge(data + half_n_data + 1, n_data - half_n_data, is_ascending);
                }
        }

        template <typename T>
        void sort (T *data, size_t n_data) {
                for (size_t k = 2; k <= n_data; k *= 2) {
                        for (size_t j = k / 2; j > 0; j = j / 2) {
                                for (size_t i = 0; i < n_data; ++i) {
                                        size_t ixj = i ^ j;
                                        if (ixj > i) {
                                                if (((i & k) == 0 && data[i] > data[ixj]) || 
                                                    ((i & k) != 0 && data[i] < data[ixj])) 
                                                        std::swap(data[i], data[ixj]);
                                        }
                                }
                        }
                }
        }
}
