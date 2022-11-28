#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <string>
#include <png++/png.hpp>
#include "vec2.h"
#include <omp.h>
#include <math.h>

#define RAYCOUNT 10000 // rays per source
#define HIT_THRESHOLD 1000

#define L1_THRESHOLD 1
#define L2_THRESHOLD 170000

const float incrRay = 0.1f;
const float degradeFac = 0.98f;
const float addFac = 100.f;

struct ray 
{
  Vec2<float> position;
  Vec2<float> velocity;
  float intensity;
  png::rgb_pixel color;
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
void updateRay(lightray *source, lightray *mRay, shifter *shifts, int cols) {
  if (!(almostEqual(mRay->position.x, source->position.x) && almostEqual(mRay->position.y, source->position.y))) {
    int accessRow = (int)mRay->position.y;
    int accessCol = (int)mRay->position.x;

    shifter *accessPoint = shifts + (accessRow * cols) + accessCol;
    
    accessPoint->blueshift += mRay->intensity;
    accessPoint->redshift += mRay->intensity;
    accessPoint->greenshift += mRay->intensity;
  }
  // update position and intensity
  //mRay->intensity *= degradeFac;
  Vec2f delta = mRay->velocity * incrRay;
  mRay->position.x += delta.x;
  mRay->position.y += delta.y;
  float normDist = mRay->position.dist(source->position) / incrRay; // normalized distance
  mRay->intensity = addFac * source->intensity / (normDist * normDist);
  return;
}

float computeContributionFactor(float score) {
  const float tuningCnst = 0.0087209302; // constants derived by fitting to logarithmic function
  const float tuningScale = 0.0820614573;
  if (score < L1_THRESHOLD) return 0.f;
  else if (score < L2_THRESHOLD) return tuningCnst + tuningScale * log(score);
  else return 1.f;
}

// takes in <color.png> <traced.png> and produces an <output.png>
int main(int argc, char** argv) {
  if (argc != 3) {
    printf("Usage: raytrace <(color)*.png> <(traced)*.png> ");
    return 1;
  } 

  const float defaultDistPixel = 1.0f;
  //Vec2f lightSourcePos(141.3f, 52.84f);
  Vec2f lightSourcePos(200.3f, 52.84f);
  lightray lightSource, lightSource1, lightSource2;
  lightSource.position = lightSourcePos;
  lightSource.velocity = lightSourcePos;
  lightSource.intensity = 255 + 255 + 255;

  lightSource1.position = Vec2f(100.66f, 210.55f);
  lightSource1.velocity = lightSource1.position;
  lightSource1.intensity = 50;

  lightSource2.position = Vec2f(200.66f, 210.55f);
  lightSource2.velocity = lightSource2.position;
  lightSource2.intensity = 300;

  std::vector<lightray> lightSources;

  // default code will have three light sources
  lightSources.push_back(lightSource);
  lightSources.push_back(lightSource1);
  

  
  png::rgb_pixel oneLight = png::rgb_pixel(255, 0, 0);
    
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
  //#pragma omp parallel for
  for (int i = 0; i < RAYCOUNT; i++) {
    Vec2f curDir(unitVecX);
    double rotAmt = (double)i * (360.0 / RAYCOUNT);
    curDir.rotate(rotAmt);
    lightray mRay;
    mRay.position = lightSourcePos;
    mRay.velocity = curDir;
    mRay.intensity = oneLight.red + oneLight.blue + oneLight.green;

    while (rayInBox(tl, br, mRay)) {
      if (tracedImg[(int)mRay.position.y][(int)mRay.position.x] == 0) break;
      else {
        updateRay(&lightSource, &mRay, (shifter*)shifts, cols); // update the ray
      }
    }
  }

  // apply source light to image
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      int contributionRed = (oneLight.red - colorImg[r][c].red) * computeContributionFactor(shifts[r][c].redshift); //std::min(1.0f, shifts[r][c].redshift / HIT_THRESHOLD);
      int contributionGreen = (oneLight.green - colorImg[r][c].green) * computeContributionFactor(shifts[r][c].greenshift); //std::min(1.0f, shifts[r][c].greenshift / HIT_THRESHOLD);
      int contributionBlue = (oneLight.blue - colorImg[r][c].blue) * computeContributionFactor(shifts[r][c].blueshift); //std::min(1.0f, shifts[r][c].blueshift / HIT_THRESHOLD);

      int newRed = std::min(255, colorImg[r][c].red + contributionRed);
      int newBlue = std::min(255, colorImg[r][c].blue + contributionBlue);
      int newGreen = std::min(255, colorImg[r][c].green + contributionGreen);
      
      //apply
      colorImg[r][c] = png::rgb_pixel(newRed, newGreen, newBlue);
    }
  }

  //printf("score at up: %f\n", shifts[0][141].redshift);
  colorImg.write("output.png"); // finished product
  return 0;
}