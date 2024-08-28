#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

enum Exit{
    SUCCESS,
    FAIL
};

struct Matrix{
    int* data;
    size_t rows, cols;
};

int printMatrix(const Matrix matrix);
int summerSchoolFootball(const int* data, const size_t teams_amount, int team1, int team2);
void swap(int* num1, int* num2);
Matrix sumMatrix(const Matrix matrix1, const Matrix matrix2);
int rowColMult(const int* row, const int* col, const size_t first_matrix_width, const size_t second_matrix_width);
Matrix multiplicationMatrix(const Matrix matrix1, const Matrix matrix2);
int printTriangleMatrix(const int* matrix, const size_t teams_amount);
int determinant(Matrix matrix);

const size_t ROWS = 3;
const size_t COLS = 2;
const size_t  FOOTBALL_TEAMS_AMOUNT = 5;
const size_t  FOOTBALL_GAMES_AMOUNT = (FOOTBALL_TEAMS_AMOUNT - 1) * FOOTBALL_TEAMS_AMOUNT / 2;

int main(void){
    int matrix_basic1[ROWS][COLS] = {{1, 2},
                                    {3, 4},
                                    {5, 6}};
    int matrix_basic2[COLS][ROWS] = {{1, 2, 3},
                                    {4, 5, 6}};
    Matrix matrix1 = {.data = (int*) matrix_basic1, .rows = ROWS, .cols = COLS};
    Matrix matrix2 = {.data = (int*) matrix_basic2, .rows = COLS, .cols = ROWS};
    printMatrix(matrix1);
    printf("\n");

    int football[FOOTBALL_GAMES_AMOUNT] = {0, 1, 1, 0, 0, 1, 1, 0, 1, 0};
    int team1 = 1;
    int team2 = 0;
    summerSchoolFootball(football, FOOTBALL_TEAMS_AMOUNT, team1, team2);
    printTriangleMatrix(football, FOOTBALL_TEAMS_AMOUNT);
    printf("\n");

    Matrix matrix_sum_result = sumMatrix(matrix1, matrix1);
    printMatrix(matrix_sum_result);
    printf("\n");

    Matrix matrix_mult_result = multiplicationMatrix(matrix1, matrix2);
    printMatrix(matrix_mult_result);
    printf("\n");

    *(matrix_mult_result.data + 4) = 27;
    int det = determinant(matrix_mult_result);
    printf("Определитель - %d\n", det);

    free(matrix_sum_result.data);
    free(matrix_mult_result.data);
}

int printMatrix(Matrix matrix){
    if (matrix.data == NULL){
        return FAIL;
    }

    for (size_t y = 0; y < matrix.rows; y++){
        for (size_t x = 0; x < matrix.cols; x++){
            assert(y >= 0 && y < matrix.rows);
            assert(x >= 0 && x < matrix.cols);

            printf("data[%lu][%lu] = %d, ", y, x, *(matrix.data + y * matrix.cols + x));
        }
        printf("\n");
    }
    return SUCCESS;
}

int summerSchoolFootball(const int* data, const size_t teams_amount, int team1, int team2){
    assert(data != NULL);

    if (team1 == team2 || team1 < 0 || team1 >= teams_amount || team2 < 0 || team2 >= teams_amount){
        printf("Такой игры нет\n");
        return FAIL;
    }

    if (team1 < team2){
        swap(&team1, &team2);
    }

    if (*(data + (team1 - 1) * team1 / 2 + team2)){
        printf("Команда %d победила команду %d\n", team1, team2);
    }
    else{
        printf("Команда %d проиграла команде %d\n", team1, team2);
    }
    return SUCCESS;
}

void swap(int* num1, int* num2){
    int tmp = *num1;
    *num1 = *num2;
    *num2 = tmp;
}
Matrix sumMatrix(const Matrix matrix1, const Matrix matrix2){
    assert(matrix1.data != NULL);
    assert(matrix2.data != NULL);

    Matrix matrix_result = {.data = NULL, .rows = matrix1.rows, .cols = matrix1.cols};
    matrix_result.data = (int*)calloc(matrix1.rows * matrix1.cols, sizeof(int));

    if (matrix1.cols != matrix2.cols || matrix1.rows != matrix2.rows){
        printf("Такие матрицы нельзя складывать\n");
        Matrix error = {.data = NULL, .rows = 0, .cols = 0};
        return error;
    }

    for (size_t y = 0; y < matrix1.rows; y++){
        for (size_t x = 0; x < matrix1.cols; x++){
            assert(y >= 0 && y < matrix1.rows);
            assert(x >= 0 && x < matrix1.cols);

            *(matrix_result.data + y * matrix1.cols + x) = *(matrix1.data + y * matrix1.cols + x) + *(matrix2.data + y * matrix2.cols + x);
        }
    }

    return matrix_result;
}

Matrix multiplicationMatrix(const Matrix matrix1, const Matrix matrix2){
    assert(matrix1.data != NULL);
    assert(matrix2.data != NULL);

    Matrix matrix_result = {.data = NULL, .rows = matrix1.rows, .cols = matrix2.cols};
    matrix_result.data = (int*)calloc(matrix1.rows * matrix2.cols, sizeof(int));

    if (matrix1.cols != matrix2.rows){
        printf("Такие матрицы нельзя перемножать\n");
        Matrix error = {.data = NULL, .rows = 0, .cols = 0};
        return error;
    }

    for (size_t y = 0; y < matrix1.rows; y++){
        for (size_t x = 0; x < matrix2.cols; x++){
            assert(y >= 0 && y < matrix1.rows);
            assert(x >= 0 && x < matrix2.cols);

            *(matrix_result.data + y * matrix2.cols + x) = rowColMult(matrix1.data + y * matrix1.cols, matrix2.data + x, matrix1.cols, matrix2.cols);
        }
    }

    return matrix_result;
}

int rowColMult(const int* row, const int* col, const size_t first_matrix_width, const size_t second_matrix_width){
    assert(row != NULL);
    assert(col != NULL);

    int result = 0;
    for(size_t elem = 0; elem < first_matrix_width; elem++){
        result += (*(row + elem)) * (*(col + elem * second_matrix_width));
    }

    return result;
}

int printTriangleMatrix(const int* matrix, const size_t teams_amount){
    assert(matrix != NULL);

    for (size_t y = 0; y < teams_amount; y++){
        for (size_t x = 0; x < teams_amount; x++){
            if (y > x){
                if (*(matrix + (y - 1) * y / 2 + x)){
                    printf("Команда %lu : Команда %lu - 1 : 0\t", y, x);
                }
                else{
                    printf("Команда %lu : Команда %lu - 0 : 1\t", y, x);
                }
            }
        }
        printf("\n");
    }

    return SUCCESS;
}

int determinant(Matrix matrix){
    assert(matrix.data != NULL);

    int result = 0;

    if (matrix.cols == 2){
        return (*(matrix.data)) * (*(matrix.data + 3)) - (*(matrix.data + 1)) * (*(matrix.data + 2));
    }

    for (size_t y = 0; y < matrix.rows; y++){
        Matrix minor = {.data = NULL, .rows = matrix.rows - 1, .cols = matrix.cols - 1};
        int tmp[minor.rows][minor.cols];
        for (size_t y_minor = 0; y_minor < minor.rows; y_minor++){
            for (size_t x_minor = 0; x_minor < minor.cols; x_minor++){
                if (y_minor < y){
                    *((int*)tmp + y_minor * minor.cols + x_minor) = *(matrix.data + y_minor * matrix.cols + x_minor + 1);
                }
                else{
                    *((int*)tmp + y_minor * minor.cols + x_minor) = *(matrix.data + (y_minor + 1) * matrix.cols + x_minor + 1);
                }
            }
        }
        minor.data = (int*) tmp;
        result += (*(matrix.data + y * matrix.cols)) * ((y % 2 == 0) ? 1 : -1) * determinant(minor);
    }

    return result;
}
