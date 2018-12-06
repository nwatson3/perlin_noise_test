#include "heightmap.h"
#include "perlin.h"
#include "matrix.h"
#include <math.h>
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;
using std::max;
using std::min;

Heightmap::Heightmap(int size, PerlinGenerator &generator)
: height(Matrix(size, size)),
  water(Matrix(size,size)),
  x_vel(Matrix(size,size)),
  y_vel(Matrix(size,size)),
  left_flux(Matrix(size,size)),
  right_flux(Matrix(size,size)),
  top_flux(Matrix(size,size)),
  bottom_flux(Matrix(size,size))
{
  this->size = size;
  
  float max_value = 0;
  float min_value = 1;

  for(int i = 0; i < size; i++)
  {
    for(int j = 0; j < size; j++)
    {
      water[i][j] = 0;
      height[i][j] = generator.perlin(i, j);
      max_value = (max_value < height[i][j]) ? height[i][j] : max_value;
      min_value = (min_value > height[i][j]) ? height[i][j] : min_value;
      /*if(height[i][j] < .5)
      {
        height[i][j] *= .25;
      }*/
    }
  }
  cout << min_value << " " << max_value << endl;
}


Matrix& Heightmap::getHeight()
{
  return height;
}

Matrix& Heightmap::getWater()
{
  return water;
}

void Heightmap::erode()
{
  float rain_amount = 0.1;
  float time_step = 0.01;
  float distance = 1.0;
  float gravity = 1.0;
  
  for(int i = 0; i < size; i++)
  {
    for(int j = 0; j < size; j++)
    {
      water[i][j] += rain_amount;
    }
  }

  for(int i = 0; i < size; i++)
  {
    for(int j = 0; j < size; j++)
    {
      float cross_sectional_pipe_area = distance * water[i][j];

      float left_height_diff =  height[i][j]   + water[i][j] 
                              - height[i-1][j] - water[i-1][j];
      left_flux[i][j] = max(0.0f, left_flux[i][j] + 
          time_step * cross_sectional_pipe_area * gravity * left_height_diff 
          / distance);

      float right_height_diff =  height[i][j]   + water[i][j] 
                              - height[i+1][j] - water[i+1][j];
      right_flux[i][j] = max(0.0f, right_flux[i][j] + 
          time_step * cross_sectional_pipe_area * gravity * right_height_diff 
          / distance);

      float up_height_diff =  height[i][j]   + water[i][j] 
                              - height[i][j+1] - water[i][j+1];
      top_flux[i][j] = max(0.0f, top_flux[i][j] + 
          time_step * cross_sectional_pipe_area * gravity * up_height_diff 
          / distance);

      float down_height_diff =  height[i][j]   + water[i][j] 
                              - height[i][j-1] - water[i][j-1];
      bottom_flux[i][j] = max(0.0f, bottom_flux[i][j] + 
          time_step * cross_sectional_pipe_area * gravity * down_height_diff 
          / distance);

      float scaling_factor = min(1.0f, water[i][j] * distance * distance 
          / ((left_flux[i][j] + 
            right_flux[i][j] + 
            top_flux[i][j] + 
            bottom_flux[i][j]) / time_step));

      right_flux[i][j] *= scaling_factor;
      left_flux[i][j] *= scaling_factor;
      top_flux[i][j] *= scaling_factor;
      bottom_flux[i][j] *= scaling_factor;

      float volume_diff;
      if(i == 0)
      {
        if(j == 0)
        {
          volume_diff = time_step * 
            (bottom_flux[i][j+1] + left_flux[i+1][j] 
             - top_flux[i][j] - right_flux[i][j]);
        }
        else if(j == size - 1)
        {
          volume_diff = time_step * 
            (top_flux[i][j-1] + left_flux[i+1][j] 
             - bottom_flux[i][j] - right_flux[i][j]);
        }
        else
        {
          volume_diff = time_step *
            (bottom_flux[i][j+1] + right_flux[i-1][j] + top_flux[i][j-1]
             - top_flux[i][j] - left_flux[i][j] - bottom_flux[i][j]);
        }
      }
      else if(i == size - 1)
      {
        if(j == 0)
        {
          volume_diff = time_step *
            (right_flux[i-1][j] + bottom_flux[i][j+1]
             - left_flux[i][j] - top_flux[i][j]);
        }
        else if(j == size - 1)
        {
          volume_diff = time_step *
            (right_flux[i-1][j] + top_flux[i][j-1]
             - left_flux[i][j] - bottom_flux[i][j]);
        }
        else
        {
          volume_diff = time_step *
            (right_flux[i-1][j] + bottom_flux[i][j+1] + left_flux[i+1][j]
             - left_flux[i][j] - top_flux[i][j] - right_flux[i][j]);
        }
      }
      else
      {
        volume_diff = time_step *
          (right_flux[i-1][j] + bottom_flux[i][j+1] + left_flux[i+1][j] + top_flux[i][j-1]
           - right_flux[i][j] - top_flux[i][j] - bottom_flux[i][j] - left_flux[i][j]);
      }

    }
  }
}

/*
void Heightmap::erode()
{
  Matrix temp_water(size, size);
  Matrix temp_sediment(size, size);
  Matrix temp_x_vel(size, size);
  Matrix temp_y_vel(size, size);

  //rain
  //get current velocity
  //determine direction/velocity water should move in
  //pick up sediment proportional to amount of water times change in velocity
  //move water to next location, along with its velocity and sediment
  //finalize the water height and velocity in new location and drop sediment
  //evaporate

  float rain_amount = 0.5;
  float solubility = 0.1;
  float friction = 0.5;
  
  // rain
  for(int i = 0; i < size; i++)
  {
    for(int j = 0; j < size; j++)
    {
      water[i][j] += rain_amount;
    }
  }

  for(int i = 0; i < size; i++)
  {
    for(int j = 0; j < size; j++)
    {
      float delta_x_vel = 0;
      float delta_y_vel = 0;
      for(int x = -1; x <= 1; x++)
      {
        for(int y = -1; y <= 1; y++)
        {
          if(i+x < size && j+y < size && i+x >= 0 && j+y >= 0)
          {
            delta_x_vel += x * (height[i][j] - height[i+x][j+y]);
            delta_y_vel += y * (height[i][j] - height[i+x][j+y]);
          }
        }
      }

      float vel_diff = sqrt(
          (x_vel[i][j] - delta_x_vel) * (x_vel[i][j] - delta_x_vel) +
          (y_vel[i][j] - delta_y_vel) * (y_vel[i][j] - delta_y_vel)
          );
      
      float sed_pickup = solubility * water[i][j] * vel_diff;

      height[i][j] -= sed_pickup;

      float new_x_vel = x_vel[i][j] + delta_x_vel;
      float new_y_vel = y_vel[i][j] + delta_y_vel;
      
      int next_x = (float)i + new_x_vel;
      int next_y = (float)j + new_y_vel;
        
      temp_x_vel[next_x][next_y] += new_x_vel;
      temp_y_vel[next_x][next_y] += new_y_vel;
      temp_sediment[next_x][next_y] += sed_pickup;
      temp_water[next_x][next_y] += water[i][j];

    }
  }

  for(int i = 0; i < size; i++)
  {
    for(int j = 0; j < size; j++)
    {
      x_vel[i][j] = temp_x_vel[i][j];
      temp_x_vel[i][j] = 0;
      y_vel[i][j] = temp_y_vel[i][j];
      temp_y_vel[i][j] = 0;
      water[i][j] = temp_water[i][j];
      temp_water[i][j] = 0;
      height[i][j] += temp_sediment[i][j];
      temp_sediment[i][j] = 0;

      // evaporate
      water[i][j] -= rain_amount;
      if(water[i][j] < 0)
      {
        water[i][j] = 0;
      }
    }
  }

}
*/
