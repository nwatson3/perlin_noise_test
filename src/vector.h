#ifndef VECTOR_H
#define VECTOR_H 

class Vector
{
  private:

    float *values;
    int size;

    float garbage;

  public:

    Vector();
    ~Vector();
    Vector(Vector &source);
    Vector(Vector &&source);
    Vector& operator=(Vector &source);
    Vector& operator=(Vector &&source);
    float& operator[](int index);

    void initialize(int size);

};

#endif /* VECTOR_H */
