void swap (__global int *rhs, __global int *lhs)
{
        int temp = *rhs;
        atomic_xchg(rhs, *lhs);
        atomic_xchg(lhs, temp);
}

__kernel void bitonic_sort (__global int *data, int n_data)
{
        int gid   = get_global_id(0);
        int gsize = get_global_size(0);

        for (int k = 2; k <= n_data; k *= 2) {
                for (int j = k / 2; j > 0; j = j / 2) {
                        for (int i = gid; i < n_data; i += gsize) {
                                int ixj = i ^ j;
                                if ((ixj) > i) {
                                        if (((i & k) == 0 && data[i] > data[ixj]) || 
                                            ((i & k) != 0 && data[i] < data[ixj])) {
                                                swap(data + i, data + ixj);
                                        }
                                }
                        }
                }
        }
}