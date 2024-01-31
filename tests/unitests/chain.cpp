#include "config.hpp"

#include "chain.hpp"

using namespace matrixes;

TEST (matrix_chain_optimal_sequences, linear1)
{
        std::vector<size_t> data {10, 20, 30, 40, 30};

        std::list<size_t> ans_data {0, 1, 2, 3};

        matrix_chain_t<double> chain {};
        chain.find_optimal_sequence(data);
        auto ans = chain.get_optimal_sequence();

        ASSERT_TRUE(ans == ans_data);
}

TEST (matrix_chain_optimal_sequences, linear2)
{
        std::vector<size_t> data {10, 30, 5, 60};

        std::list<size_t> ans_data {0, 1, 2};

        matrix_chain_t<double> chain {};
        chain.find_optimal_sequence(data);
        auto ans = chain.get_optimal_sequence();

        ASSERT_TRUE(ans == ans_data);
}

TEST (matrix_chain_optimal_sequences, not_linear1)
{
        std::vector<size_t> data {30, 35, 15, 5, 10};

        std::list<size_t> ans_data {1, 2, 0, 3};

        matrix_chain_t<double> chain {};
        chain.find_optimal_sequence(data);
        auto ans = chain.get_optimal_sequence();

        ASSERT_TRUE(ans == ans_data);
}

TEST (matrix_chain_optimal_sequences, not_linear2)
{
        std::vector<size_t> data {40, 20, 30, 10, 30};

        std::list<size_t> ans_data {1, 2, 0, 3};

        matrix_chain_t<double> chain {};
        chain.find_optimal_sequence(data);
        auto ans = chain.get_optimal_sequence();

        ASSERT_TRUE(ans == ans_data);
}

TEST (matrix_chain_dummy_mul, E_E_mul)
{
        std::vector data {1, 0, 2, 
                          0, 3, 0,
                          4, 0, 5};
        std::vector E_data {1, 0, 0,
                            0, 1, 0,
                            0, 0, 1};

        matrix_t<int> matrix {3, 3, data.begin()};

        std::vector<std::shared_ptr<imatrix_t>> matrixes {std::shared_ptr<imatrix_t>{new matrix_t<int> {matrix}}, std::shared_ptr<imatrix_t>{new matrix_t<int> {3, 3, E_data.begin()}}, 
                                                                                      std::shared_ptr<imatrix_t>{new matrix_t<int> {3, 3, E_data.begin()}}};
        EXPECT_EQ(matrix[0].get_size(), 3);
        matrix_chain_t<int> chain {matrixes};

        std::shared_ptr<imatrix_t> ans_matrix = chain.dummy_mul();

        EXPECT_EQ(ans_matrix->get_n_cols(), 3);
        EXPECT_EQ(ans_matrix->get_n_rows(), 3);

        ASSERT_TRUE(matrix == *static_cast<matrix_t<int>*>(ans_matrix.get()));
}

TEST (matrix_chain_opt_mul, E_E_mul)
{
        std::vector data {1, 0, 2, 
                          0, 3, 0,
                          4, 0, 5};
        std::vector E_data {1, 0, 0,
                            0, 1, 0,
                            0, 0, 1};

        matrix_t<int> matrix {3, 3, data.begin()};

        std::vector<std::shared_ptr<imatrix_t>> matrixes {std::shared_ptr<imatrix_t>{new matrix_t<int> {matrix}}, std::shared_ptr<imatrix_t>{new matrix_t<int> {3, 3, E_data.begin()}}, 
                                                                                      std::shared_ptr<imatrix_t>{new matrix_t<int> {3, 3, E_data.begin()}}};
        EXPECT_EQ(matrix[0].get_size(), 3);
        matrix_chain_t<int> chain {matrixes};

        std::shared_ptr<imatrix_t> ans_matrix = chain.optimal_mul();

        EXPECT_EQ(ans_matrix->get_n_cols(), 3);
        EXPECT_EQ(ans_matrix->get_n_rows(), 3);

        ASSERT_TRUE(matrix == *static_cast<matrix_t<int>*>(ans_matrix.get()));
}

TEST (matrix_chain_opt_mul, O_O_mul)
{
        std::vector data {1, 0, 2, 
                          0, 3, 0,
                          4, 0, 5};
        std::vector O_data {0, 0, 0,
                            0, 0, 0,
                            0, 0, 0};

        matrix_t<int> matrix {3, 3, data.begin()};
        matrix_t<int> O_matrix {3, 3, O_data.begin()};

        std::vector<std::shared_ptr<imatrix_t>> matrixes {std::shared_ptr<imatrix_t>{new matrix_t<int> {matrix}}, std::shared_ptr<imatrix_t>{new matrix_t<int> {3, 3, O_data.begin()}}, 
                                                                                      std::shared_ptr<imatrix_t>{new matrix_t<int> {3, 3, O_data.begin()}}};
        EXPECT_EQ(matrix[0].get_size(), 3);
        matrix_chain_t<int> chain {matrixes};

        std::shared_ptr<imatrix_t> ans_matrix = chain.optimal_mul();

        EXPECT_EQ(ans_matrix->get_n_cols(), 3);
        EXPECT_EQ(ans_matrix->get_n_rows(), 3);

        ASSERT_TRUE(O_matrix == *static_cast<matrix_t<int>*>(ans_matrix.get()));
}

TEST (matrix_chain_opt_mul, two_diff_matrixes)
{
        std::vector data {1, 2, 3, 
                          4, 5, 6,
                          0, 0, 0};
        std::vector data2  {1, 1,
                            1, 1,
                            1, 1};
        std::vector ans_data {6,  6,
                              15, 15,
                              0,  0};

        matrix_t<int> matrix  {3, 3, data.begin()};
        matrix_t<int> matrix2 {3, 2, data2.begin()};

        matrix_t<int> ans {3, 2, ans_data.begin()};

        std::vector<std::shared_ptr<imatrix_t>> matrixes {std::shared_ptr<imatrix_t>{new matrix_t<int> {matrix}}, std::shared_ptr<imatrix_t>{new matrix_t<int> {matrix2}}};
        matrix_chain_t<int> chain {matrixes};

        std::shared_ptr<imatrix_t> ans_matrix = chain.optimal_mul();

        EXPECT_EQ(ans_matrix->get_n_cols(), 2);
        EXPECT_EQ(ans_matrix->get_n_rows(), 3);

        ASSERT_TRUE(ans == *static_cast<matrix_t<int>*>(ans_matrix.get()));
}

TEST (matrix_chain_opt_mul, tree_diff_matrixes)
{
        std::vector data {1, 0, 1, 
                          5, 6, 5,
                          0, 2, 0};
        std::vector data2  {1, 4,
                            2, 5,
                            3, 6};
        std::vector data3  {1,
                            1};
        std::vector ans_data {14,
                              112,
                              14};

        matrix_t<int> matrix  {3, 3, data.begin()};
        matrix_t<int> matrix2 {3, 2, data2.begin()};
        matrix_t<int> matrix3 {2, 1, data3.begin()};

        matrix_t<int> ans {3, 1, ans_data.begin()};

        std::vector<std::shared_ptr<imatrix_t>> matrixes {std::shared_ptr<imatrix_t>{new matrix_t<int> {matrix}},
                                                          std::shared_ptr<imatrix_t>{new matrix_t<int> {matrix2}},
                                                          std::shared_ptr<imatrix_t>{new matrix_t<int> {matrix3}}};
        matrix_chain_t<int> chain {matrixes};

        std::shared_ptr<imatrix_t> ans_matrix = chain.optimal_mul();

        EXPECT_EQ(ans_matrix->get_n_cols(), 1);
        EXPECT_EQ(ans_matrix->get_n_rows(), 3);

        // static_cast<matrix_t<int>*>(ans_matrix.get())->dump();

        ASSERT_TRUE(ans == *static_cast<matrix_t<int>*>(ans_matrix.get()));
}

TEST (matrix_chain_opt_mul, tree_diff_matrixes_not_linear)
{
        std::vector data {1, 
                          2,
                          3, 
                          4};
        std::vector data2  {5, 6, 7, 8, 9, 10};
        std::vector data3  {1,
                            2,
                            3,
                            4,
                            5,
                            6};
        std::vector ans_data {175,
                              350,
                              525,
                              700};

        matrix_t<int> matrix  {4, 1, data.begin()};
        matrix_t<int> matrix2 {1, 6, data2.begin()};
        matrix_t<int> matrix3 {6, 1, data3.begin()};

        matrix_t<int> ans {4, 1, ans_data.begin()};

        std::vector<std::shared_ptr<imatrix_t>> matrixes {std::shared_ptr<imatrix_t>{new matrix_t<int> {matrix}},
                                                          std::shared_ptr<imatrix_t>{new matrix_t<int> {matrix2}},
                                                          std::shared_ptr<imatrix_t>{new matrix_t<int> {matrix3}}};
        matrix_chain_t<int> chain {matrixes};

        std::shared_ptr<imatrix_t> ans_matrix = chain.optimal_mul();

        // static_cast<matrix_t<int>*>(ans_matrix.get())->dump();

        ASSERT_TRUE(ans == *static_cast<matrix_t<int>*>(ans_matrix.get()));
}

TEST (matrix_chain_opt_mul, E_E_matrixes_not_linear)
{
        std::vector data {1, 0, 0, 0, 0, 
                          0, 1, 0, 0, 0,
                          0, 0, 1, 0, 0, 
                          0, 0, 0, 1, 0,
                          0, 0, 0, 0, 1};
        std::vector data2  {1,
                            2,
                            3,
                            4,
                            5};
        std::vector ans_data {1,
                              2,
                              3,
                              4,
                              5};
        matrix_t<int> matrix  {5, 5, data.begin()};
        matrix_t<int> matrix2 {5, 5, data.begin()};
        matrix_t<int> matrix3 {5, 1, data2.begin()};

        matrix_t<int> ans {5, 1, ans_data.begin()};

        std::vector<std::shared_ptr<imatrix_t>> matrixes {std::shared_ptr<imatrix_t>{new matrix_t<int> {matrix}},
                                                          std::shared_ptr<imatrix_t>{new matrix_t<int> {matrix2}},
                                                          std::shared_ptr<imatrix_t>{new matrix_t<int> {matrix3}}};
        matrix_chain_t<int> chain {matrixes};

        std::shared_ptr<imatrix_t> ans_matrix = chain.optimal_mul();

        // static_cast<matrix_t<int>*>(ans_matrix.get())->dump();

        ASSERT_TRUE(ans == *static_cast<matrix_t<int>*>(ans_matrix.get()));
}