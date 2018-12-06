#ifndef PERLIN_H
#define PERLIN_H 

#include <vector>

class PerlinGenerator
{
  private:

    struct Octave
    {
      float influence;
      int square_size;
    };

    std::vector<Octave> octaves;

    float ***gradient;

    int grid_size;

    float dotGridGradient(int ix, int iy, float x, float y) const;

  public:

    PerlinGenerator(int grid_size);

    ~PerlinGenerator();

    PerlinGenerator(PerlinGenerator &source);
    PerlinGenerator(PerlinGenerator &&source);

    PerlinGenerator& operator=(PerlinGenerator &source);
    PerlinGenerator& operator=(PerlinGenerator &&source);

    void addOctave(float influence, int square_size);

    float perlin(float input_x, float input_y);

};

#endif /* PERLIN_H */
