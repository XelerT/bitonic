/*
        1 Make the four-element subsequences bitonic by comparing each pair of elements. 
                Even-numbered pairs are sorted in ascending order. Odd-numbered
                pairs are assorted in descending order.
        2 Continue making larger bitonic subsequences (8-element, 16-element, and so
                on) by comparing and swapping elements in the lower half with elements in the
                upper half. If you remember the hill shape we’re looking for, you’ll know when
                to sort upward and when to sort downward.
        3 After creating a bitonic subsequence, sort it using the bitonic merge. This isn’t
                necessary for four-element subsequences.

        ■ bitonic_sort_init — At the start of the sort, each work-item reads two vectors and sorts
                their components. Then the work-items in the work-group sort every data point
                assigned to the group.
        
        ■ bitonic_sort_stage_n — This kernel performs higher stages of the bitonic sort, which
                form the data points into a bitonic set.
        
        ■ bitonic_sort_stage_0 — Each higher stage requires that every lower stage be executed.
                This kernel corresponds to the bottom stage of the sort.
        
        ■ bitonic_sort_merge — Once the data points have been sorted into a bitonic set, this
                kernel places the data points in ascending or descending order.
        
        ■ bitonic_sort_merge_last — This kernel performs the final sorting of the work-groups’
                data elements.
*/

#define SORT_VECTOR(input, dir)                                 \
        comp = abs(input > shuffle(input, mask2)) ^ dir;        \
        input = shuffle(input, comp * 2 + add2);                \
        comp = abs(input > shuffle(input, mask1)) ^ dir;        \
        input = shuffle(input, comp + add1);

#define SWAP_VECTORS(in1, in2, dir)                             \
        input1 = in1;                                           \
        input2 = in2;                                           \
        comp = (abs(input1 > input2) ^ dir) * 4 + add3;         \
        in1 = shuffle2(input1, input2, comp);                   \
        in2 = shuffle2(input2, input1, comp);    

void swap (__global int *rhs, __global int *lhs)
{
        int temp = *rhs;
        atomic_xchg(rhs, *lhs);
        atomic_xchg(lhs, temp);
}

// __kernel void bitonic_sort (__global int4 *g_data, __local int4 *l_data, uint k, uint j)
// {
//         int gid   = get_global_id(0);
//         int gsize = get_global_size(0);
        
//         for (int i = gid; i < n_data; i += gsize) {
//                 int ixj = i ^ j;
//                 if ((ixj) > i) {
//                         if (((i & k) == 0 && data[i] > data[ixj]) || 
//                             ((i & k) != 0 && data[i] < data[ixj])) {
//                                 swap(data + i, data + ixj);
//                         }
//                 }
//         }
// }

__kernel void bitonic_sort_stage_n (__global int *g_data, uint k, uint j) 
{
        int4 input1, input2, temp;
        uint4 comp, add3;
        uint id, global_offset;
        int idxj, dir, data1, data2;

        // uint4  swap, mask1, mask2, add1, add2;
        // mask1 = (uint4)(1, 0, 3, 2);
        // swap = (uint4)(0, 0, 1, 1);
        // add1 = (uint4)(0, 0, 2, 2);
        // mask2 = (uint4)(2, 3, 0, 1);
        // add2 = (uint4)(0, 1, 0, 1);
        // add3 = (uint4)(0, 1, 2, 3);

        id = get_group_id(0) + (get_group_id(0) / j) * j * get_local_size(0) + 
             get_local_id(0);

        global_offset = j * get_local_size(0);

        // printf("id gl_off loc_id j %d %d %zu %d\n", id, global_offset, get_local_id(0), j);
        idxj = id ^ j;

        data1 = g_data[id];
        data2 = g_data[idxj];

        if (idxj > id) {
                if (((id & k) == 0 && data1 > data2) || 
                    ((id & k) != 0 && data1 < data2)) {
                        swap(g_data + id, g_data + idxj);
                }
        }

        // input1 = g_data[id];
        // input2 = g_data[id + global_offset];

        // dir = get_local_id(0) / k & 1;

        // temp = input1;
        // add3 = (uint4) (0, 1, 2, 3);

        // SORT_VECTOR(input1, dir)
        // SORT_VECTOR(input2, dir)
        // SWAP_VECTORS(input1, input2, dir)

        // comp = (abs(input1 < input2) ^ dir) * 4 + add3;
        // printf("before %d %d\n", input1, input2);
        // g_data[id]                 = shuffle2(input1, input2, comp);
        // printf("after %d %d\n", input1, input2);
        // g_data[id + global_offset] = shuffle2(input2, temp, comp);
}

/* Sort elements within a vector */
#define VECTOR_SORT(input, dir)                                        \
    comp = input < shuffle(input, mask2) ^ dir;                        \
    input = shuffle(input, as_uint4(comp * 2 + add2));                 \
    comp = input < shuffle(input, mask1) ^ dir;                        \
    input = shuffle(input, as_uint4(comp + add1));                     \

#define VECTOR_SWAP(input1, input2, dir)                               \
    temp = input1;                                                     \
    comp = (input1 < input2 ^ dir) * 4 + add3;                         \
    input1 = shuffle2(input1, input2, as_uint4(comp));                 \
    input2 = shuffle2(input2, temp, as_uint4(comp));                   \

void last_merge_sort (__global int4 *g_data, __local int4 *l_data, int dir)
{
        uint id, global_start, stride;
    int4 input1, input2, temp;
    int4 comp;

    uint4 mask1 = (uint4)(1, 0, 3, 2);
    uint4 mask2 = (uint4)(2, 3, 0, 1);
    uint4 mask3 = (uint4)(3, 2, 1, 0);

    int4 add1 = (int4)(1, 1, 3, 3);
    int4 add2 = (int4)(2, 3, 2, 3);
    int4 add3 = (int4)(4, 5, 6, 7);

    /* Determine location of data in global memory */
    id = get_local_id(0);
    global_start = get_group_id(0) * get_local_size(0) * 2 + id;

    /* Perform initial swap */
    input1 = g_data[global_start];
    input2 = g_data[global_start + get_local_size(0)];
    comp = (input1 < input2 ^ dir) * 4 + add3;
    l_data[id] = shuffle2(input1, input2, as_uint4(comp));
    l_data[id + get_local_size(0)] = shuffle2(input2, input1, as_uint4(comp));

    /* Perform bitonic merge */
    for(stride = get_local_size(0)/2; stride > 1; stride >>= 1)
    {
        barrier(CLK_LOCAL_MEM_FENCE);
        id = get_local_id(0) + (get_local_id(0)/stride)*stride;
        VECTOR_SWAP(l_data[id], l_data[id + stride], dir)
    }
    barrier(CLK_LOCAL_MEM_FENCE);

    /* Perform final sort */
    id = get_local_id(0) * 2;
    input1 = l_data[id]; input2 = l_data[id+1];
    temp = input1;
    comp = (input1 < input2 ^ dir) * 4 + add3;
    input1 = shuffle2(input1, input2, as_uint4(comp));
    input2 = shuffle2(input2, temp, as_uint4(comp));
    VECTOR_SORT(input1, dir);
    VECTOR_SORT(input2, dir);

    /* Store the result to global memory */
    g_data[global_start + get_local_id(0)] = input1;
    g_data[global_start + get_local_id(0) + 1] = input2;
}

__kernel void bitonic_sort_stage_0 (__global int4 *g_data, __local int4 *l_data, uint k)
{
        int dir = (get_group_id(0) / k & 1) * -1;
        
        last_merge_sort(g_data, l_data, dir);
}

__kernel void bitonic_sort_merge_last (__global int4 *g_data, __local int4 *l_data)
{
        int dir = 0;
        last_merge_sort(g_data, l_data, dir);
}