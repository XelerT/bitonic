#include "config.hpp"

using namespace matrixes;

TEST (row, init)
{
        std::vector data {1, 2, 3, 4};

        row_t<int> row {data.begin(), data.end()};

        EXPECT_EQ(row.get_size(), 4);

        EXPECT_EQ(row[0], 1);
        EXPECT_EQ(row[1], 2);
        EXPECT_EQ(row[2], 3);
        EXPECT_EQ(row[3], 4);
}

TEST (row, copy_ctor)
{
        std::vector data {1, 2, 3, 4};

        row_t<int> row {data.begin(), data.end()};
        row_t cp_row {row};

        EXPECT_EQ(cp_row.get_size(), 4);

        EXPECT_EQ(cp_row[0], 1);
        EXPECT_EQ(cp_row[1], 2);
        EXPECT_EQ(cp_row[2], 3);
        EXPECT_EQ(cp_row[3], 4);
}

TEST (row, copy_ass)
{
        std::vector data {1, 2, 3, 4};
        std::vector data2 {4, 5, 9};

        row_t<int> row {data.begin(), data.end()};
        row_t<int> cp_row {data2.begin(), data2.end()};
        cp_row = row;

        EXPECT_EQ(cp_row.get_size(), 4);

        EXPECT_EQ(cp_row[0], 1);
        EXPECT_EQ(cp_row[1], 2);
        EXPECT_EQ(cp_row[2], 3);
        EXPECT_EQ(cp_row[3], 4);
}
