#include "matrix.h"
#include "vector.h"
#include <iostream>
#include <stdexcept>

Matrix::Matrix(int width, int height)
{
  garbage.initialize(0);

  this->width = width;
  this->height = height;

  columns = new Vector[width];

  for(int i = 0; i < width; i++)
  {
    columns[i].initialize(height);
  }
}

Matrix::~Matrix()
{
  delete[] columns;
}

Matrix::Matrix(Matrix &source)
{
  width = source.width;
  height = source.height;

  columns = new Vector[width];

  for(int i = 0; i < width; i++)
  {
    columns[i].initialize(height);
    for(int j = 0; j < height; j++)
    {
      columns[i][j] = source[i][j];
    }
  }
}

Matrix::Matrix(Matrix &&source)
{
  width = source.width;
  height = source.height;
  columns = source.columns;

  source.columns = nullptr;
}

Matrix& Matrix::operator=(Matrix &source)
{
  width = source.width;
  height = source.height;
  for(int i = 0; i < width; i++)
  {
    for(int j = 0; j < height; j++)
    {
      columns[i][j] = source[i][j];
    }
  }

  return *this;
}

Matrix& Matrix::operator=(Matrix &&source)
{
  width = source.width;
  height = source.height;
  columns = source.columns;

  source.columns = nullptr;

  return *this;
}

Vector& Matrix::operator[](int index)
{
  if(index < 0 || index >= width)
  {
    return garbage;
  }
  
  //Vector column = columns[index];

  return columns[index];
}
