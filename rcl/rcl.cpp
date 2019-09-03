#include <emscripten.h>

#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>
#include <cfloat>

#include "utils.h"
#include "vec3.h"
#include "raytracer.h"
#include "objects.h"
#include "texture.h"

using namespace std;

int WIDTH, HEIGHT;
const int MAX_DEPTH = 50;

vec3 color(const Ray& r, Hitable& world, int depth=0) {
  Hit h = world.hit(r, 0.001, FLT_MAX);
  if (h) {
    Scatter s = h.material->scatter(r, h);
    if (depth < MAX_DEPTH && s) {
      return s.attenuation * color(s.scattered, world, depth + 1);
    } else {
      return vec3(0, 0, 0);
    }
  }

  vec3 dir = unit(r.direction);
  float t = 0.5 * (dir.y() + 1.0);
  return (1.0 - t) * vec3(1., 1., 1.) + t * vec3(0.5, 0.7, 1.0);
}

extern "C" {

EMSCRIPTEN_KEEPALIVE
void setup(int width, int height, string seedrandom) {
  WIDTH = width;
  HEIGHT = height;

  srand (static_cast <unsigned> (time(0)));

}

EMSCRIPTEN_KEEPALIVE
unsigned char* render(int x0, int y0, int width, int height, int sampling) {
  sampling = max(1, sampling);
  unsigned char* ret = (unsigned char*)malloc(width * height * 4);

  vec3 llc(-2., -1., -1.);
  vec3 hor(4., 0., 0.);
  vec3 ver(0., 2., 0.);
  vec3 origin(0., 0., 0.);

  HitList world;

  world.add(Sphere::create(vec3(0, 0, -1), 0.5, Lambertian::create(vec3(0.1, 0.2, 0.5))));
  world.add(Sphere::create(vec3(0, -100.5, -1), 100, Lambertian::create(vec3(0.8, 0.8, 0.0))));
  world.add(Sphere::create(vec3(1, 0, -1), 0.5, Metal::create(vec3(0.8, 0.6, 0.2), 0.0)));
  world.add(Sphere::create(vec3(-1, 0, -1), 0.5, Dielectric::create(1.5)));

  Camera cam;

  int p = 0;
  for (int j = height - 1; j >= 0; --j) {
    for (int i = 0; i < width; ++i) {
      int x = x0 + i;
      int y = HEIGHT - y0 - height + j;

      vec3 col(0, 0, 0);
      for (int s = 0; s < sampling; ++s) {
        float u = float(x + frand()) / WIDTH;
        float v = float(y + frand()) / HEIGHT;

        Ray r = cam.getRay(u, v);
        col += color(r, world);
      }

      col /= sampling;

      ret[p++] = int(255.99 * sqrt(col.x()));
      ret[p++] = int(255.99 * sqrt(col.y()));
      ret[p++] = int(255.99 * sqrt(col.z()));
      ret[p++] = 255;
    }

  }
  assert(p == width * height * 4);
  return ret;
}

} // extern
