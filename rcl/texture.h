#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "utils.h"
#include "vec3.h"
#include "raytracer.h"

struct Scatter {
  Ray scattered;
  vec3 attenuation;

  operator bool() const {
    return scattered.direction[0] != 0 || scattered.direction[1] != 0 || scattered.direction[2] != 0;
  }
};

Scatter NoScatter{Ray{vec3(0, 0, 0), vec3(0, 0, 0)}, vec3(0, 0, 0)};

class Material {
public:
  virtual Scatter scatter(const Ray& ray, const Hit& hit) const = 0;
};

class Lambertian : public Material {
public:
  static shared_ptr<Lambertian> create(vec3 a) {
    return shared_ptr<Lambertian>(new Lambertian(a));
  }

  Lambertian(const vec3& a) : albedo(a) { }

  virtual Scatter scatter(const Ray& ray, const Hit& hit) const {
    vec3 target = hit.p + hit.normal + randomInUnitSphere();
    return Scatter{Ray(hit.p, target - hit.p), albedo};
  }

  vec3 albedo;
};

class Metal : public Material {
public:
  static shared_ptr<Metal> create(vec3 a, float f) {
    return shared_ptr<Metal>(new Metal(a, f));
  }

  Metal(const vec3& a, float f) : albedo(a), fuzz(f) { }

  virtual Scatter scatter(const Ray& ray, const Hit& hit) const {
    vec3 reflected = reflect(unit(ray.direction), hit.normal);
    Ray scattered(hit.p, reflected + fuzz * randomInUnitSphere());
    if (dot(scattered.direction, hit.normal) > 0) {
      return Scatter{scattered, albedo};
    }
    return NoScatter;
  }

  vec3 albedo;
  float fuzz;
};

class Dielectric : public Material {
public:
  static shared_ptr<Dielectric> create(float ri) {
    return shared_ptr<Dielectric>(new Dielectric(ri));
  }

  Dielectric(float ri) : refidx(ri) { }

  virtual Scatter scatter(const Ray& ray, const Hit& hit) const {
    vec3 reflected = reflect(ray.direction, hit.normal);
    float nint;
    vec3 outnormal;
    if (dot(ray.direction, hit.normal) > 0) {
      outnormal = -hit.normal;
      nint = refidx;
    } else {
      outnormal = hit.normal;
      nint = 1. / refidx;
    }

    vec3 refracted = refract(ray.direction, outnormal, nint);
    Ray scattered;
    if (refracted) {
      scattered = Ray(hit.p, refracted);
    } else {
      scattered = Ray(hit.p, reflected);
    }
    return Scatter{scattered, vec3(1, 1, 1)};
  }

  float refidx;
};

#endif
