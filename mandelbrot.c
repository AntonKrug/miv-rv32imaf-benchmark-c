/*******************************************************************************
 * (c) Copyright 2016-2017 Microsemi SoC Products Group. All rights reserved.
 * v1.0-noOutput 2017/12/19 anton.krug@microsemi.com
 * Reference: https://rosettacode.org/wiki/Mandelbrot_set
 */

#include <stdint.h>
#include <float.h>
#include <math.h>

#define WIDTH 110
#define HEIGHT 40

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

struct MandelbrotView {
  float lookAtX;
  float lookAtY;
  float width;
  float height;
  float gamma;
};

struct MandelbrotView sets[] = {
  {-0.5f,          0.0f,        2.5f,          3.0f,            1.0f},
  {-1.2059523f,   -0.34705183f, 0.36000158f,   0.17550077f,     2.0f},
  {-0.55f,        -0.64f,       0.15f,         0.115f,          4.0f},
  { 0.5f,          0.0f,        1.0f,          2.0f,            1.0f},
  {-0.0068464412f,-0.80686056f, 0.0160606767f, 0.00782957993f, 12.0f}
};


uint32_t mandelbrot(float lookAtX, float lookAtY, float width, float height, float gamma) {
  const float xmin    = lookAtX - (width  / 2);
  const float ymin    = lookAtY - (height / 2);
  const float stepX   = width  / WIDTH;
  const float stepY   = height / HEIGHT;
  const int   maxIter = (20.0f * gamma);
  uint32_t sum = 0;

  for (int cursorY = 2; cursorY < HEIGHT; cursorY++) {
    const float y = ymin + cursorY * stepY;

    for (int cursorX = 0; cursorX < WIDTH; cursorX++) {
      const float x = xmin + cursorX * stepX;
      float u  = 0.0f;
      float v  = 0.0f;
      float u2 = 0.0f;
      float v2 = 0.0f;
      int iter;

      for (iter = 0 ; iter < maxIter && ( u2+v2 < 4.0f); iter++) {
        v  = 2 * (u*v) + y;
        u  = (u2) - (v2) +x;
        u2 = u * u;
        v2 = v * v;
      }

      sum += (iter >= maxIter) ? 0 : (int)(iter/gamma);
    }
  }
  return sum;
}


float rescale(float old, float new, float percentage) {
  return ((new - old) * fminf(1.0f, fmaxf(0.0f, percentage))) + old;
}


int allMandebrotSets() {
  int sum = 0;

  for (int i = 0 ; i < (NELEMS(sets) - 1); i++) {
    for (float percentage = 0.0f; percentage <= 1.3f; percentage += 0.04f) {
      const int   iNext   = (i +1) % NELEMS(sets);
      const float lookAtX = rescale(sets[i].lookAtX, sets[iNext].lookAtX, percentage);
      const float lookAtY = rescale(sets[i].lookAtY, sets[iNext].lookAtY, percentage);
      const float width   = rescale(sets[i].width,   sets[iNext].width,   percentage);
      const float height  = rescale(sets[i].height,  sets[iNext].height,  percentage);
      const float gamma   = rescale(sets[i].gamma,   sets[iNext].gamma,   percentage);

      sum += mandelbrot(lookAtX, lookAtY, width, height, gamma);
    }
  }
  return sum;
}

