#include "vector.h"
#include <iostream>

void Vector::initialize(int size)
{
  this->size = size;
  values = new float[size];
  for(int i = 0; i < size; i++)
  {
    values[i] = 0;
  }
}

Vector::Vector()
{

}

Vector::~Vector()
{
  delete[] values;
}

Vector::Vector(Vector &source)
{
  size = source.size;
  for(int i = 0; i < size; i++)
  {
    values[i] = source.values[i];
  }
}

Vector::Vector(Vector &&source)
{
  size = source.size;
  values = source.values;

  source.values = nullptr;
}

Vector& Vector::operator=(Vector &source)
{
  size = source.size;
  for(int i = 0; i < size; i++)
  {
    values[i] = source.values[i];
  }

  return *this;
}

Vector& Vector::operator=(Vector &&source)
{
  size = source.size;
  values = source.values;

  source.values = nullptr;

  return *this;
}

float& Vector::operator[](int index)
{
  if(index < 0 || index >= size)
  {
    return garbage;
  }
  return values[index];
}
