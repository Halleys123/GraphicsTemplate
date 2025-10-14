#pragma once

template <typename T>
class Matrix
{
public:
    // By default row matrix calculations and result save one extra matrix just for giving it to opengl
    T mat[4][4];

public:
    Matrix();
    Matrix(T values[4][4]);
    T *operator[](int row);
    Matrix operator*(Matrix &other);
    Matrix transpose();

    // Static factory methods for common transformations
    // static Matrix identity();
    static Matrix perspective(T fov, T aspect, T near, T far);
    // static Matrix lookAt(T eyeX, T eyeY, T eyeZ, T centerX, T centerY, T centerZ, T upX, T upY, T upZ);
};