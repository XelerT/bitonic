#include "config.hpp"

using namespace std;
using namespace matrixes;

TEST (sq_matrix, determinant2)
{
        std::vector data {3.0, 4.0, 1.0, 3.0};

        sq_matrix_t<double> matrix {2, data.begin()};

        auto det_status = matrix.det();
        EXPECT_EQ(det_status.second, status_t::OK);
        EXPECT_NEAR(det_status.first, 5, THRESHOLD);
}

TEST (sq_matrix, determinant3)
{
        std::vector<double> data {1, 2, 3, 4, 5, 6, 7, 8, 9};

        sq_matrix_t<double> matrix {3, data.begin()};

        auto det_status = matrix.det();
        EXPECT_EQ(det_status.second, status_t::DEGENERATED);
        EXPECT_NEAR(det_status.first, 0, THRESHOLD);
}

// TEST (sq_matrix, determinant5)
// {
//         std::vector<double> data {-3614400, -576000, 574848, -177152, -6594, -2700000, -430560,
//                                    429696, -132608, -4928, -2239200, -357600, 357120, -110336, 
//                                   -4095, -720000, -114240, 114048, -34816, -1309, -887400, 
//                                   -141600, 141696, -43520, -1624};

//         sq_matrix_t<double> matrix {5, data.begin()};

//         auto det_status = matrix.det();
//         EXPECT_EQ(det_status.second, status_t::OK);
//         EXPECT_NEAR(det_status.first, 1783627776000, THRESHOLD);
// }

TEST (sq_matrix, determinant5_0)
{
        std::vector<double> data {1080000, 0, 28000, -420, 0, 0, 0, 0, 
                                  -420, 0, 0, 0, 0, -420, 61440, 2160000, 
                                  0, 84000, -420, 0, 0, 0, 0, -420, 0};

        sq_matrix_t<double> matrix {5, data.begin()};

        auto det_status = matrix.det();
        EXPECT_EQ(det_status.second, status_t::DEGENERATED);
        EXPECT_NEAR(det_status.first, 0, THRESHOLD);
}