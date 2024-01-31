#include "config.hpp"

using namespace matrixes;
using namespace std;

TEST (matrix, init)
{
        std::vector data {1, 2, 3, 4};

        matrix_t<int> matrix {2, 2, data.begin()};

        EXPECT_EQ(matrix.get_n_cols(), 2);
        EXPECT_EQ(matrix.get_n_rows(), 2);
        EXPECT_EQ(matrix.get_n_cols(), matrix[0].get_size());

        EXPECT_EQ(matrix[0][0], 1);
        EXPECT_EQ(matrix[0][1], 2);
        EXPECT_EQ(matrix[1][0], 3);
        EXPECT_EQ(matrix[1][1], 4);
}

TEST (matrix, copy_ctor)
{
        std::vector data {1, 2, 3, 4};

        matrix_t<int> matrix {2, 2, data.begin()};
        matrix_t cp_matrix {matrix};

        EXPECT_EQ(cp_matrix.get_n_cols(), 2);
        EXPECT_EQ(cp_matrix.get_n_rows(), 2);

        EXPECT_EQ(cp_matrix[0][0], 1);
        EXPECT_EQ(cp_matrix[0][1], 2);
        EXPECT_EQ(cp_matrix[1][0], 3);
        EXPECT_EQ(cp_matrix[1][1], 4);
}

TEST (matrix, copy_ass)
{
        std::vector data {1, 2, 3, 4};
        std::vector data2 {4, 5, 9, 7, 1, 2};

        matrix_t<int> matrix {2, 2, data.begin()};
        matrix_t<int> cp_matrix {3, 2, data2.begin()};
        cp_matrix = matrix;

        EXPECT_EQ(cp_matrix.get_n_cols(), 2);
        EXPECT_EQ(cp_matrix.get_n_rows(), 2);

        EXPECT_EQ(cp_matrix[0][0], 1);
        EXPECT_EQ(cp_matrix[0][1], 2);
        EXPECT_EQ(cp_matrix[1][0], 3);
        EXPECT_EQ(cp_matrix[1][1], 4);
}

TEST (matrix, equal_oper_diff_sizes)
{
        std::vector data {1, 2, 3, 4};
        std::vector data2 {4, 5, 9, 7, 1, 2};

        matrix_t<int> matrix  {2, 2, data.begin()};
        matrix_t<int> matrix2 {3, 2, data2.begin()};
        
        ASSERT_FALSE(matrix == matrix2);
}

TEST (matrix, equal_oper_diff_values)
{
        std::vector data  {1, 2, 3, 4, 0, 0};
        std::vector data2 {4, 5, 9, 7, 1, 2};

        matrix_t<int> matrix  {3, 2, data.begin()};
        matrix_t<int> matrix2 {3, 2, data2.begin()};
        
        ASSERT_FALSE(matrix == matrix2);
}

TEST (matrix, equal_oper_equal_matrixes)
{
        std::vector data  {1, 2, 3, 4, 0, 0};
        std::vector data2 {1, 2, 3, 4, 0, 0};

        matrix_t<int> matrix  {3, 2, data.begin()};
        matrix_t<int> matrix2 {3, 2, data2.begin()};
        
        ASSERT_TRUE(matrix == matrix2);
}

TEST (matrix, multiply_E) 
{
        std::vector data {1, 0, 2, 
                          0, 3, 0,
                          4, 0, 5};
        std::vector E_data {1, 0, 0,
                            0, 1, 0,
                            0, 0, 1};

        matrix_t<int> matrix {3, 3, data.begin()};
        matrix_t<int> matrix_E {3, 3, E_data.begin()};

        shared_ptr<matrix_t<int>> ans {static_cast<matrix_t<int>*>(matrix.mul(matrix_E))};
        ASSERT_TRUE(*ans == matrix);
}