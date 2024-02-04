# Bitonic sorting using OpenCL

Implementation without using local memory. 

## Install & Build

        $ git clone https://github.com/XelerT/bitonic.git
        $ cd bitonic
        $ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build

## Run
We have some options in working modes of program. After running program you need to enter number of data elements to sort (N) and then elements themself.

In ```build/```:

        $ ./bitonic_sort
        $ N ...

Available options:

        Allowed options:
                 -h [ --help ]         display help info and exit
                --globsz arg          Set global size
                --locsz arg           Set local size
                -k [ --kernel ] arg   Set path to the kernel file
                -o [ --offset ] arg   Set kernel offset in queue
                --compare arg         Turn on comparison sorts: 
                                                                                        
                                                cpu  - bitonic sort using cpu,
                                                                                        
                                                sort - std sort.
                --time                Print consumed time to sort data.
                --print arg           Print sorted data (default value: true)
