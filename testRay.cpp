#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <string>
#include <png++/png.hpp>
#include "vec2.h"

struct ray 
{
  Vec2<float> position;
  Vec2<float> velocity;
};

struct lightcollector
{
  float redshift;
  float greenshift;
  float blueshift;
};

typedef struct ray lightray;
typedef struct lightcollector shifter;

bool rayInBox(Vec2<float> tl, Vec2<float> br, lightray mRay) {
  Vec2<float> brHead = br - (mRay.position);
  Vec2<float> tlHead = (mRay.position) - tl;
  return (tlHead.x >= 0 && tlHead.y >= 0 && brHead.x >= 0 && brHead.y >= 0);
}

// takes in <color.png> <traced.png> and produces an <output.png>
int main(int argc, char** argv) {
  if (argc != 3) {
    printf("Usage: raytrace <(color)*.png> <(traced)*.png> ");
    return 1;
  } 
    
  png::image<png::rgb_pixel> colorImg(argv[1]);
  png::image<png::gray_pixel> tracedImg(argv[2]);
  int rows = colorImg.get_height();
  int cols = colorImg.get_width();

  if (rows != tracedImg.get_height() && cols != tracedImg.get_width()) {
    printf("Error: Color Image and Traced Image are not of same dimension\n");
    return 2;
  }

  shifter shifts[rows][cols]; // don't forget to initalize
  // this will collect the "update score" for each pixel from all of the rays that pass through

  

  colorImg.write("output.png"); // finished product
  return 0;
}