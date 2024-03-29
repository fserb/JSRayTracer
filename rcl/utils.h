#ifndef __UTILS_H__
#define __UTILS_H__

#include "vec3.h"

inline float frand() {
  return static_cast<float>(rand()) / static_cast <float> (RAND_MAX);
}

vec3 getSphereUV(const vec3& normal) {
  float phi = atan2(normal.z(), normal.x());
  float theta = asin(normal.y());
  return vec3(1 - (phi + M_PI) / (2 * M_PI),
    (theta + M_PI / 2) / M_PI, 0);
}

vec3 randomInUnitSphere() {
  vec3 p = unit(vec3(frand(), frand(), frand())) * cbrt(frand());
  return (p * 2.0) - 1.0;
}

vec3 randomInUnitDisk() {
  float r = sqrt(frand());
  float t = frand() * 2. * M_PI;
  return vec3(r * cos(t), r * sin(t), 0.0);
}

#endif
