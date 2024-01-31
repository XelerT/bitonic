import random
import numpy as np

GEN_DATA_MAX = 0x5


def create_sq_matrix(diagonal):
        matrix = np.array([np.array([0 for i in range(len(diagonal))]) for i in range(len(diagonal))], dtype=np.float64)
        for row, i in zip(matrix, range(len(diagonal))):
                row[i] = diagonal[i]

        return matrix


def swap_rows(diagonal_matrix, n_row_swaps):
        for i in range(n_row_swaps):
                n_row1 = random.randint(0, len(diagonal_matrix) - 1)
                n_row2 = n_row1
                while (n_row1 == n_row2):
                        n_row2 = random.randint(0, len(diagonal_matrix) - 1)

                tmp = np.array(diagonal_matrix[n_row1])
                diagonal_matrix[n_row1] = diagonal_matrix[n_row2]
                diagonal_matrix[n_row2] = tmp
                
        if (n_row_swaps % 2 != 0):
                return -1.0
        return 1.0


def add_rows(diagonal_matrix, n_add_rows):
        for i in range(n_add_rows):
                n_row1 = random.randint(0, len(diagonal_matrix) - 1)
                n_row2 = n_row1
                while (n_row1 == n_row2):
                        n_row2 = random.randint(0, len(diagonal_matrix) - 1)

                diagonal_matrix[n_row1] += diagonal_matrix[n_row2]


def multiply_matrix_rows(diagonal_matrix, n_mult_rows):
        determinant_mult_coeff = 1.0
        for i in range(n_mult_rows):
                n_row = random.randint(0, len(diagonal_matrix) - 1)
                coeff = float(random.randint(1, 5))
                if (random.randint(0, 1)):
                        determinant_mult_coeff *= coeff
                        diagonal_matrix[n_row] *= coeff
                else:
                        determinant_mult_coeff /= coeff
                        diagonal_matrix[n_row] /= coeff


        return determinant_mult_coeff 


def mix_matrix(diagonal_matrix, determinant):
        n_row_swaps = random.randint(1, len(diagonal_matrix[0]) * 10)
        n_add_rows  = random.randint(1, len(diagonal_matrix[0]) * 10)
        n_mult_rows = random.randint(1, len(diagonal_matrix[0]) * 10)

        determinant *= swap_rows(diagonal_matrix, n_row_swaps)
        determinant *= multiply_matrix_rows(diagonal_matrix, n_mult_rows)
        add_rows(diagonal_matrix, n_add_rows)

        return diagonal_matrix, determinant


def gen_sq_matrix (size):
        diagonal = [ float(random.randint(-GEN_DATA_MAX, GEN_DATA_MAX)) for i in range(size) ]

        determinant = 1.0
        for i in range(size):
                determinant *= diagonal[i]

        matrix, determinant = mix_matrix(create_sq_matrix(diagonal), determinant)

        return matrix, determinant


def gen_matrix (n_rows, n_cols):
        # elems = [ random.randint(-GEN_DATA_MAX, GEN_DATA_MAX) for i in range(n_rows * n_cols) ]
        matrix = np.array([np.array([random.randint(-GEN_DATA_MAX, GEN_DATA_MAX) for i in range(n_cols)]) for i in range(n_rows)], dtype=np.int64)
        
        # for row, i in zip(matrix, range(n_rows)):
        #         for elem, j in zip(row, range(n_cols)):
        #                 elem = elems[i * n_cols + j]

        return matrix