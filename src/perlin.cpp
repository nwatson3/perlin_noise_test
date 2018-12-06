#include "perlin.h"
#include "ofMain.h"
#include <math.h>
#include <iostream>

float fade(float t) {
   return t * t * t * (t * (t * 6 - 15) + 10);
}

// Function to linearly interpolate between a0 and a1
// Weight w should be in the range [0.0, 1.0]
float lerp(float a0, float a1, float w) {
    return fade(1.0 - w)*a0 + fade(w)*a1;

    // as an alternative, this slightly faster equivalent formula can be used:
    // return a0 + w*(a1 - a0);
}

// Computes the dot product of the distance and gradient vectors.
float PerlinGenerator::dotGridGradient(int ix, int iy, float x, float y) const {

    // Precomputed (or otherwise) gradient vectors at each grid node
    //extern float Gradient[IYMAX][IXMAX][2];

    // Compute the distance vector
    float dx = x - (float)ix;
    float dy = y - (float)iy;

    // Compute the dot-product
    return (dx*gradient[iy][ix][0] + dy*gradient[iy][ix][1]);
}

// Compute Perlin noise at coordinates x, y
float PerlinGenerator::perlin(float input_x, float input_y) {

  float total_influence = 0;
  float total_value = 0;
  for(Octave octave : octaves)
  {
    float x = input_x / (float)octave.square_size;
    float y = input_y / (float)octave.square_size;

    // Determine grid cell coordinates
    int x0 = int(x);
    int x1 = x0 + 1;
    int y0 = int(y);
    int y1 = y0 + 1;

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    float sx = x - (float)x0;
    float sy = y - (float)y0;

    // Interpolate between grid point gradients
    float n0, n1, ix0, ix1, value;
    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = lerp(n0, n1, sx);
    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = lerp(n0, n1, sx);
    value = (lerp(ix0, ix1, sy) / (2*sqrt(0.5))) + 0.5;

    total_value += octave.influence * value;

    total_influence += octave.influence;
  }

  return total_value / total_influence;
}

void PerlinGenerator::addOctave(float influence, int square_size)
{
  Octave octave;
  octave.influence = influence;
  octave.square_size = square_size;
  octaves.push_back(octave);
}

PerlinGenerator::PerlinGenerator(int grid_size)
{
  this->grid_size = grid_size;

  gradient = new float**[grid_size];
  for(int i = 0; i < grid_size; i++)
  {
    gradient[i] = new float *[grid_size];
    for(int j = 0; j < grid_size; j++)
    {
      gradient[i][j] = new float[2];
      gradient[i][j][0] = ofRandom(-1, 1);
      gradient[i][j][1] = ofRandom(-1, 1);

      float length = sqrt(gradient[i][j][0] * gradient[i][j][0]
                          + gradient[i][j][1] * gradient[i][j][1]);

      gradient[i][j][0] /= length;
      gradient[i][j][1] /= length;
    }
  }
}

PerlinGenerator::~PerlinGenerator()
{
  for(int i = 0; i < grid_size; i++)
  {
    for(int j = 0; j < grid_size; j++)
    {
      delete[] gradient[i][j];
    }
    delete[] gradient[i];
  }
  delete[] gradient;
}

PerlinGenerator::PerlinGenerator(PerlinGenerator &source)
{
  grid_size = source.grid_size;

  gradient = new float**[grid_size];
  for(int i = 0; i < grid_size; i++)
  {
    gradient[i] = new float *[grid_size];
    for(int j = 0; j < grid_size; j++)
    {
      gradient[i][j][0] = source.gradient[i][j][0];
      gradient[i][j][1] = source.gradient[i][j][1];
    }
  }
}

PerlinGenerator& PerlinGenerator::operator=(PerlinGenerator &source)
{
  grid_size = source.grid_size;

  gradient = new float**[grid_size];
  for(int i = 0; i < grid_size; i++)
  {
    gradient[i] = new float *[grid_size];
    for(int j = 0; j < grid_size; j++)
    {
      gradient[i][j][0] = source.gradient[i][j][0];
      gradient[i][j][1] = source.gradient[i][j][1];
    }
  }

  return *this;
}

PerlinGenerator::PerlinGenerator(PerlinGenerator &&source)
{
  grid_size = source.grid_size;
  gradient = source.gradient;

  for(int i = 0; i < grid_size; i++)
  {
    for(int j = 0; j < grid_size; j++)
    {
      source.gradient[i][j] = nullptr;
    }
    source.gradient[i] = nullptr;
  }
  source.gradient = nullptr;
  source.grid_size = 0;
}

PerlinGenerator& PerlinGenerator::operator=(PerlinGenerator &&source)
{
  grid_size = source.grid_size;
  gradient = source.gradient;

  for(int i = 0; i < grid_size; i++)
  {
    for(int j = 0; j < grid_size; j++)
    {
      source.gradient[i][j] = nullptr;
    }
    source.gradient[i] = nullptr;
  }
  source.gradient = nullptr;
  source.grid_size = 0;

  return *this;
}
