#ifndef _MATRIX_H_
#define _MATRIX_H_

//
// originally implemented by Justin Legakis
//

#include <math.h>
#include <assert.h>

#include "vectors.h"

// ====================================================================
// ====================================================================

class Matrix {

public:

    // CONSTRUCTORS & DESTRUCTOR
    Matrix() { Clear(); }
    Matrix(const Matrix& m);
    Matrix(const float* m);
    ~Matrix() {}

    // ACCESSORS
    float* glGet(void) const;
    float Get(int x, int y) const;

    // MODIFIERS
    void Set(int x, int y, float v);
    void SetToIdentity();
    void Clear();

    void Transpose(Matrix& m) const;
    void Transpose();

    int Inverse(Matrix& m, float epsilon = 1e-08) const;
    int Inverse(float epsilon = 1e-08);

    // OVERLOADED OPERATORS
    Matrix& operator=(const Matrix& m);
    int operator==(const Matrix& m) const;
    int operator!=(const Matrix& m) const;
    friend Matrix operator+(const Matrix& m1, const Matrix& m2);
    friend Matrix operator-(const Matrix& m1, const Matrix& m2);
    friend Matrix operator*(const Matrix& m1, const Matrix& m2);
    friend Matrix operator*(const Matrix& m1, float f);
    friend Matrix operator*(float f, const Matrix& m);
    Matrix& operator+=(const Matrix& m);
    Matrix& operator-=(const Matrix& m);
    Matrix& operator*=(const float f);
    Matrix& operator*=(const Matrix& m);

    // TRANSFORMATIONS
    static Matrix MakeTranslation(const Vec3f& v);
    static Matrix MakeScale(const Vec3f& v);
    static Matrix MakeScale(float s);
    static Matrix MakeXRotation(float theta);
    static Matrix MakeYRotation(float theta);
    static Matrix MakeZRotation(float theta);
    static Matrix MakeAxisRotation(const Vec3f& v, float theta);

    // Use to transform a point with a matrix
    // that may include translation
    void Transform(Vec4f& v) const;
    void Transform(Vec3f& v) const;
    void Transform(Vec2f& v) const;

    // Use to transform the direction of the ray
    // (ignores any translation)
    void TransformDirection(Vec3f& v) const;

    // INPUT / OUTPUT
    void Write(FILE* F = stdout) const;
    void Write3x3(FILE* F = stdout) const;
    void Read(FILE* F);
    void Read3x3(FILE* F);

private:

    // REPRESENTATION
    float	data[4][4];

};

// ====================================================================
// ====================================================================

#endif