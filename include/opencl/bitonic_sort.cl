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
__kernel void bitonic_sort_stage_n (__global int *g_data, uint k, uint j) 
{
        int4 input1, input2, temp;
        uint4 comp, add3;
        uint id, global_offset;
        int idxj, dir, data1, data2;

        id = get_group_id(0) + (get_group_id(0) / j) * j * get_local_size(0) + 
             get_local_id(0);

        global_offset = j * get_local_size(0);

        idxj = id ^ j;

        data1 = g_data[id];
        data2 = g_data[idxj];

        if (idxj > id) {
                if (((id & k) == 0 && data1 > data2) || 
                    ((id & k) != 0 && data1 < data2)) {
                        swap(g_data + id, g_data + idxj);
                }
        }
}