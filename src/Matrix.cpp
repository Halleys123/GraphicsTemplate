#include "Matrix.hpp"
#include "string.h"
#include "math.h"

template <typename T>
Matrix<T>::Matrix()
{
    memset(mat, 0, sizeof(T) * 16);
}

template <typename T>
Matrix<T>::Matrix(T values[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            mat[i][j] = values[i][j];
        }
    }
}

template <typename T>
T *Matrix<T>::operator[](int row)
{
    return mat[row];
}

template <typename T>
Matrix<T> Matrix<T>::operator*(Matrix &other)
{
    Matrix nMatrix;
    for (int row = 0; row < 4; row += 1)
    {
        for (int col = 0; col < 4; col += 1)
        {
            nMatrix[row][col] = 0;
            for (int i = 0; i < 4; i += 1)
            {
                nMatrix[row][col] += mat[row][i] * other[i][col];
            }
        }
    }

    return nMatrix;
}

template <typename T>
Matrix<T> Matrix<T>::transpose()
{
    Matrix nMatrix;

    for (int row = 0; row < 4; row += 1)
    {
        for (int col = 0; col < 4; col += 1)
        {
            nMatrix[row][col] = mat[col][row];
        }
    }

    return nMatrix;
}

template <typename T>
Matrix<T> Matrix<T>::perspective(T fov_deg, T aspect, T near, T far)
{
    Matrix<T> result;
    T fov = fov_deg * 3.1415f / 180.0f;
    T tanHalfFov = tan(fov / 2);

    memset(result.mat, 0, sizeof(T) * 16);

    result[0][0] = 1 / (aspect * tanHalfFov);
    result[1][1] = 1 / tanHalfFov;
    result[2][2] = -(far + near) / (far - near);
    result[2][3] = -1;
    result[3][2] = -(2 * far * near) / (far - near);

    result = result.transpose();

    return result;
}

// Explicit template instantiations
template class Matrix<float>;
template class Matrix<double>;