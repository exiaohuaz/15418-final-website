#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <string>
#include <png++/png.hpp>
#include "vec2.h"

#define RAYCOUNT 1000 // rays per source
#define HIT_THRESHOLD 30000

const float incrRay = 0.1f;
const float degradeFac = 0.99f;

struct ray 
{
  Vec2<float> position;
  Vec2<float> velocity;
  float intensity;
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

bool almostEqual(float a, float b) {
  return (abs(b - a) <= 10e-12);
}

// updates the position and intensity of a ray while contributing its partial score to the new image
void updateRay(Vec2f source, lightray *mRay, shifter *shifts, int cols) {
  if (!(almostEqual(mRay->position.x, source.x) && almostEqual(mRay->position.y, source.y))) {
    int accessRow = (int)mRay->position.y;
    int accessCol = (int)mRay->position.x;

    shifter *accessPoint = shifts + (accessRow * cols) + accessCol;
    
    accessPoint->blueshift += mRay->intensity;
    accessPoint->redshift += mRay->intensity;
    accessPoint->greenshift += mRay->intensity;
  }
  // update position and intensity
  mRay->intensity *= degradeFac;
  Vec2f delta = mRay->velocity * incrRay;
  mRay->position.x += delta.x;
  mRay->position.y += delta.y;
  return;
}

// takes in <color.png> <traced.png> and produces an <output.png>
int main(int argc, char** argv) {
  if (argc != 3) {
    printf("Usage: raytrace <(color)*.png> <(traced)*.png> ");
    return 1;
  } 

  const float defaultDistPixel = 1.0f;
  Vec2f lightSource(141.3f, 52.84f);
  png::rgb_pixel oneLight = png::rgb_pixel(255, 180, 180);
    
  png::image<png::rgb_pixel> colorImg(argv[1]);
  png::image<png::gray_pixel> tracedImg(argv[2]);
  int rows = colorImg.get_height();
  int cols = colorImg.get_width();

  if (rows != tracedImg.get_height() && cols != tracedImg.get_width()) {
    printf("Error: Color Image and Traced Image are not of same dimension\n");
    return 2;
  }


  shifter shifts[rows][cols]; // initialize shift scores per pixel
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      shifts[r][c].blueshift = 0.0f;
      shifts[r][c].redshift = 0.0f;
      shifts[r][c].greenshift = 0.0f;
    }
  }
  // this will collect the "update score" for each pixel from all of the rays that pass through

  const Vec2f tl(0.0f, 0.0f);
  const Vec2f br((float)cols, (float)rows);
  const Vec2f unitVecX(1.0f, 0.0f);

  // spawn rays from source
  for (int i = 0; i < RAYCOUNT; i++) {
    Vec2f curDir(unitVecX);
    double rotAmt = (double)i * (360.0 / RAYCOUNT);
    curDir.rotate(rotAmt);
    lightray mRay;
    mRay.position = lightSource;
    mRay.velocity = curDir;
    mRay.intensity = oneLight.red + oneLight.blue + oneLight.green;

    while (rayInBox(tl, br, mRay)) {
      if (tracedImg[(int)mRay.position.y][(int)mRay.position.x] == 0) break;
      else {
        updateRay(lightSource, &mRay, (shifter*)shifts, cols); // update the ray
      }
    }
  }

  // apply source light to image
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      int contributionRed = (oneLight.red - colorImg[r][c].red) * std::min(1.0f, shifts[r][c].redshift / HIT_THRESHOLD);
      int contributionGreen = (oneLight.green - colorImg[r][c].green) * std::min(1.0f, shifts[r][c].greenshift / HIT_THRESHOLD);
      int contributionBlue = (oneLight.blue - colorImg[r][c].blue) * std::min(1.0f, shifts[r][c].blueshift / HIT_THRESHOLD);

      int newRed = std::min(255, colorImg[r][c].red + contributionRed);
      int newBlue = std::min(255, colorImg[r][c].blue + contributionBlue);
      int newGreen = std::min(255, colorImg[r][c].green + contributionGreen);
      
      //apply
      colorImg[r][c] = png::rgb_pixel(newRed, newGreen, newBlue);
    }
  }


  colorImg.write("output.png"); // finished product
  return 0;
}