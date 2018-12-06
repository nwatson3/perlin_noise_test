#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H 

#include "perlin.h"
#include "matrix.h"

class Heightmap
{
  private:

    Matrix height;
    Matrix water;

    Matrix x_vel;
    Matrix y_vel;

    Matrix left_flux;
    Matrix right_flux;
    Matrix top_flux;
    Matrix bottom_flux;

    int size;

  public:

    Heightmap(int size, PerlinGenerator &generator);

    Matrix& getHeight();

    Matrix& getWater();

    void erode();
};

#endif /* HEIGHTMAP_H */
