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
        comp = abs (input > shuffle(input, mask2)) ^ dir;       \
        input = shuffle(input, comp * 2 + add2);                \
        comp = abs(input > shuffle(input, mask1)) ^ dir;        \
        input = shuffle(input, comp + add1);

#define SWAP_VECTORS(in1, in2, dir)                              \
        input1 = in1;                                           \
        input2 = in2;                                           \
        comp = (abs(input1 > input2) ^ dir) * 4 + add3;         \
        in1 = shuffle2(input1, input2, comp);                   \
        in2 = shuffle2(input2, input1, comp);                   

void last_merge_sort (__global int4 *g_data, __local int4 *l_data, uint dir)
{
        int4 input1, input2, temp;

        uint4 comp, swap, mask1, mask2, add1, add2, add3;
        uint id, global_start, size, stride;

        mask1 = (uint4) (1, 0, 3, 2);
        swap  = (uint4) (0, 0, 1, 1);
        add1  = (uint4) (0, 0, 2, 2);
        mask2 = (uint4) (2, 3, 0, 1);
        add2  = (uint4) (0, 1, 0, 1);
        add3  = (uint4) (0, 1, 2, 3);

        id  = get_local_id(0);

        global_start = get_group_id(0) * 
                       get_local_size(0) * 2 + id;

        input1 = g_data[global_start];
        input2 = g_data[global_start + get_local_size(0)];

        temp = input1;
        comp = (abs(input1 > input2) ^ dir) * 4 + add3;
        input1 = shuffle2(input1, input2, comp);
        input2 = shuffle2(input2, temp, comp);

        l_data[id]                     = input1;
        l_data[id + get_local_size(0)] = input2;

        for (stride = get_local_size(0); stride > 1; stride /= 2) {
                barrier(CLK_LOCAL_MEM_FENCE);
                id = get_local_id(0) + (get_local_id(0) / stride) * stride;
                SWAP_VECTORS(l_data[id], l_data[id + stride], dir)
        }
        barrier(CLK_LOCAL_MEM_FENCE);

        id = get_local_id(0) * 2;
        input1 = l_data[id]; 
        input2 = l_data[id + 1];
        temp = input1;

        comp = (abs(input1 > input2) ^ dir) * 4 + add3;
        input1 = shuffle2(input1, input2, comp);
        input2 = shuffle2(input2, temp, comp);
        SORT_VECTOR(input1, dir);
        SORT_VECTOR(input2, dir);

        g_data[global_start + get_local_id(0)]     = input1;
        g_data[global_start + get_local_id(0) + 1] = input2;
}

__kernel void bitonic_sort_init (__global int4 *g_data, __local int4 *l_data)
{
        int4 input1, input2, temp;

        uint4 comp, swap, mask1, mask2, add1, add2, add3;
        uint id, dir, global_start, size, stride;

        mask1 = (uint4) (1, 0, 3, 2);
        swap  = (uint4) (0, 0, 1, 1);
        add1  = (uint4) (0, 0, 2, 2);
        mask2 = (uint4) (2, 3, 0, 1);
        add2  = (uint4) (0, 1, 0, 1);
        add3  = (uint4) (0, 1, 2, 3);

        /* finding global address */
        id = get_local_id(0) * 2;
        global_start = get_group_id(0) * 
                       get_local_size(0) * 2 + id;
        
        input1 = g_data[global_start];
        input2 = g_data[global_start + 1];

        /* sort first vector */
        comp   = abs(input1 > shuffle(input1, mask1));
        input1 = shuffle(input1, comp ^ swap + add1);
        comp   = abs(input1 > shuffle(input1, mask2));
        input1 = shuffle(input1, comp + add1);
        comp   = abs(input1 > shuffle(input1, mask1));
        input1 = shuffle(input1, comp + add1);

        /* sort second vector */
        comp   = abs(input2 < shuffle(input2, mask1));
        input2 = shuffle(input2, comp ^ swap + add1);
        comp   = abs(input2 < shuffle(input2, mask2));
        input2 = shuffle(input2, comp + add1);
        comp   = abs(input2 < shuffle(input2, mask1));
        input2 = shuffle(input2, comp + add1);

        /* swap elements */
        dir = get_group_id(0) % 2;
        temp = input1;
        comp = (abs(input1 > input2) ^ dir) * 4 + add3;
        input1 = shuffle2(input1, input2, comp);
        input2 = shuffle2(input2, temp, comp);

        SORT_VECTOR(input1, dir);
        SORT_VECTOR(input2, dir);

        l_data[id]     = input1;
        l_data[id + 1] = input2;

        /* perform upper stage */
        for (size = 2; size < get_local_size(0); size *= 2) {
                dir = get_local_id(0) / size & 1;

                /* perform lower stages */
                for (stride = size; stride > 1; stride /= 2) {
                        barrier(CLK_LOCAL_MEM_FENCE);
                        id = get_local_id(0) + (get_local_id(0) / stride) * stride;
                        SWAP_VECTORS(l_data[id], l_data[id + stride], dir)
                }
                barrier(CLK_LOCAL_MEM_FENCE);

                id = get_local_id(0) * 2;
                input1 = l_data[id]; 
                input2 = l_data[id + 1];
                temp = input1;

                comp = (abs(input1 > input2) ^ dir) * 4 + add3;
                input1 = shuffle2(input1, input2, comp);
                input2 = shuffle2(input2, temp, comp);
                SORT_VECTOR(input1, dir);
                SORT_VECTOR(input2, dir);

                l_data[id]     = input1;
                l_data[id + 1] = input2;
        }

        dir = get_group_id(0) % 2;
        /* perform bitonic merge */
        for (stride = get_local_size(0); stride > 1; stride /= 2) {
                barrier(CLK_LOCAL_MEM_FENCE);
                id = get_local_id(0) + (get_local_id(0) / stride) * stride;
                SWAP_VECTORS(l_data[id], l_data[id + stride], dir)
        }
        barrier(CLK_LOCAL_MEM_FENCE);

        id = get_local_id(0) * 2;
        input1 = l_data[id];
        input2 = l_data[id + 1];
        temp = input1;

        comp = (abs(input1 > input2) ^ dir) * 4 + add3;
        input1 = shuffle2(input1, input2, comp);
        input2 = shuffle2(input2, temp, comp);
        SORT_VECTOR(input1, dir);
        SORT_VECTOR(input2, dir);
        
        g_data[global_start] = input1;
        g_data[global_start + 1] = input2;
}

__kernel void bitonic_sort_stage_n (__global int4 *g_data, __local int4 *l_data, int n) 
{
        int4 input1, input2, temp;
        uint4 comp, add;
        uint id, dir, global_offset;

        add  = (uint4) (0, 1, 2, 3);

        id = get_group_id(0) + (get_group_id(0) / n) * n * get_local_size(0) + 
             get_local_id(0);

        global_offset = n * get_local_size(0);

        input1 = g_data[id];
        input2 = g_data[id + global_offset];

        dir = get_local_id(0) / n & 1;

        comp = (abs(input1 > input2) ^ dir) * 4 + add;
        g_data[id]                 = shuffle2(input1, input2, comp);
        g_data[id + global_offset] = shuffle2(input2, temp, comp);
}

__kernel void bitonic_sort_stage_0 (__global int4 *g_data, __local int4 *l_data, int stage)
{

        uint dir = get_group_id(0) / stage & 1;
        
        last_merge_sort(g_data, l_data, dir);
}

__kernel void bitonic_sort_merge (__global int4 *g_data, __local int4 *l_data, int n)
{
        int4 input1, input2, temp;
        uint4 comp, add;
        uint id, global_offset;
        uint dir = 1;

        add  = (uint4) (0, 1, 2, 3);

        id = get_group_id(0) + (get_group_id(0) / n) * n * get_local_size(0) + 
             get_local_id(0);

        global_offset = n * get_local_size(0);

        input1 = g_data[id];
        input2 = g_data[id + global_offset];

        dir = get_local_id(0) / n & 1;

        comp = (abs(input1 > input2) ^ dir) * 4 + add;
        g_data[id]                 = shuffle2(input1, input2, comp);
        g_data[id + global_offset] = shuffle2(input2, temp, comp);
}

__kernel void bitonic_sort_merge_last (__global int4 *g_data, __local int4 *l_data)
{
        uint dir = 1;
        last_merge_sort(g_data, l_data, dir);
}