#ifndef MATRIX_H
#define MATRIX_H 

#include "vector.h"

class Matrix
{
  private:

    Vector *columns;
    int width;
    int height;

    Vector garbage;
    
  public:
    
    Matrix(int width, int height);
    ~Matrix();
    Matrix(Matrix &source);
    Matrix(Matrix &&source);
    Matrix& operator=(Matrix &source);
    Matrix& operator=(Matrix &&source);
    Vector& operator[](int index);

};

#endif /* MATRIX_H */
