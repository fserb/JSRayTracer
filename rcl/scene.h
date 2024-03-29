#ifndef __SCENE_H__
#define __SCENE_H__

#include "utils.h"
#include "vec3.h"
#include "raytracer.h"
#include "objects.h"
#include "texture.h"

struct Scene {
  shared_ptr<Hitable> world;
  Camera camera;
};

Scene scene1(int width, int height) {
  HitList world;

  world.add(make_shared<Sphere>(vec3(0, 0, -1), 0.5,
    make_shared<Lambertian>(make_shared<ConstantTexture>(0.1, 0.2, 0.5))));
  world.add(make_shared<Sphere>(vec3(0, -100.5, -1), 100,
    make_shared<Lambertian>(make_shared<ConstantTexture>(0.8, 0.8, 0.0))));
  world.add(make_shared<Sphere>(vec3(1, 0, -1), 0.5,
    make_shared<Metal>(make_shared<ConstantTexture>(0.8, 0.6, 0.2), 0.0)));
  world.add(make_shared<Sphere>(vec3(-1, 0, -1), 0.5, make_shared<Dielectric>(1.5)));
  world.add(make_shared<Sphere>(vec3(-1, 0, -1), -0.45, make_shared<Dielectric>(1.5)));

  vec3 lookfrom(3, 3, 2);
  vec3 lookat(0, 0, -1);
  float distToFocus = (lookfrom - lookat).length();
  float aperture = 2.0;

  Camera camera(lookfrom, lookat, vec3(0, 1, 0), 20, width / height, aperture, distToFocus);

  shared_ptr<BVH> bvh(new BVH(world.list));
  return Scene{bvh, camera};
}

Scene scene3(int width, int height) {
  HitList world;

  shared_ptr<Texture> pertext = make_shared<NoiseTexture>(5);

  shared_ptr<Texture> earth = make_shared<ImageTexture>("./textures/2k_earth_daymap.jpg");

  world.add(make_shared<Sphere>(vec3(0, -1000, 0), 1000,
    make_shared<Lambertian>(pertext)));
  world.add(make_shared<Sphere>(vec3(0, 2, 0), 2,
    make_shared<Lambertian>(earth)));

  world.add(make_shared<RectXY>(-3, 3, 1, 3, -3,
    make_shared<DiffuseLight>(make_shared<ConstantTexture>(4, 4, 4))));

  vec3 lookfrom(13, 2, 3);
  vec3 lookat(0, 0, 0);
  float distToFocus = 10.0;
  float aperture = 0.0;

  Camera camera(lookfrom, lookat, vec3(0, 1, 0), 40, width / height, aperture, distToFocus);

  shared_ptr<BVH> bvh(new BVH(world.list));
  return Scene{bvh, camera};
}


Scene scene2(int width, int height) {
  HitList world;

  shared_ptr<Texture> checker(make_shared<CheckerTexture>(
    make_shared<ConstantTexture>(0.2, 0.3, 0.1),
    make_shared<ConstantTexture>(0.9, 0.9, 0.9)));

  world.add(make_shared<Sphere>(vec3(0, -1000, 0), 1000,
    make_shared<Lambertian>(checker)));

  for (int a = -11; a < 11; ++a) {
    for (int b = -11; b < 11; ++b) {
      float mat = frand();
      vec3 center(a + 0.9 * frand(), 0.2, b + 0.9 * frand());
      if ((center - vec3(4, 0, 2.0)).length() > 0.9) {
        if (mat < 0.8) {
          world.add(make_shared<Sphere>(center, 0.2,
            make_shared<Lambertian>(
              make_shared<ConstantTexture>(frand() * frand(), frand() * frand(), frand() * frand()))));
        } else if (mat < 0.95) {
          world.add(make_shared<Sphere>(center, 0.2,
            make_shared<Metal>(make_shared<ConstantTexture>(
              0.5 * (1 + frand()),
              0.5 * (1 + frand()),
              0.5 * (1 + frand())), 0.5 * frand())));
        } else {
          world.add(make_shared<Sphere>(center, 0.2, make_shared<Dielectric>(1.5)));
        }
      }
    }
  }

  world.add(make_shared<Sphere>(vec3(0, 1, 0), 1.0, make_shared<Dielectric>(1.5)));
  world.add(make_shared<Sphere>(vec3(-4, 1, 0), 1.0,
    make_shared<Lambertian>(make_shared<ConstantTexture>(0.4, 0.2, 0.1))));
  world.add(make_shared<Sphere>(vec3(4, 1, 0), 1.0,
    make_shared<Metal>(make_shared<ConstantTexture>(0.7, 0.6, 0.5), 0.0)));

  vec3 lookfrom(13, 2, 3);
  vec3 lookat(0, 0, -1);
  float distToFocus = 10.0; //(lookfrom - lookat).length();
  float aperture = 0.1;

  Camera camera(lookfrom, lookat, vec3(0, 1, 0), 20, width / height, aperture, distToFocus);

  shared_ptr<BVH> bvh(new BVH(world.list));
  return Scene{bvh, camera};
}

Scene sceneCornell(int width, int height) {
  HitList world;

  shared_ptr<Material> red = make_shared<Lambertian>(make_shared<ConstantTexture>(0.65, 0.05, 0.05));
  shared_ptr<Material> white = make_shared<Lambertian>(make_shared<ConstantTexture>(0.73, 0.73, 0.73));
  shared_ptr<Material> green = make_shared<Lambertian>(make_shared<ConstantTexture>(0.12, 0.45, 0.15));
  shared_ptr<Material> light = make_shared<DiffuseLight>(make_shared<ConstantTexture>(7, 7, 7));

  world.add(make_shared<FlipNormals>(make_shared<RectYZ>(0, 555, 0, 555, 555, green)));
  world.add(make_shared<RectYZ>(0, 555, 0, 555, 0, red));
  world.add(make_shared<RectXZ>(123, 423, 147, 412, 554, light));
  world.add(make_shared<FlipNormals>(make_shared<RectXZ>(0, 555, 0, 555, 555, white)));
  world.add(make_shared<RectXZ>(0, 555, 0, 555, 0, white));
  world.add(make_shared<FlipNormals>(make_shared<RectXY>(0, 555, 0, 555, 555, white)));

  mat4 box1t;
  box1t.translate(130 + 82.5, 82.5, 65 + 82.5)
    .rotateXYZ(0, -M_PI / 6, 0)
    .scale(82.5);

  mat4 box2t;
  box2t.translate(265 + 82.5, 165, 295 + 82.5)
    .rotateXYZ(0, M_PI / 8, 0)
    .scale(82.5, 165, 82.5);

  shared_ptr<Hitable> b1 = make_shared<Transform>(box1t, make_shared<Box>(vec3(-1, -1, -1), vec3(1, 1, 1), white));
  shared_ptr<Hitable> b2 = make_shared<Transform>(box2t, make_shared<Box>(vec3(-1, -1, -1), vec3(1, 1, 1), white));

  world.add(make_shared<ConstantMedium>(b1, 0.01, make_shared<ConstantTexture>(1, 1, 1)));
  world.add(make_shared<ConstantMedium>(b2, 0.01, make_shared<ConstantTexture>(0, 0, 0)));

  vec3 lookfrom(278, 278, -800);
  vec3 lookat(278, 278, 0);
  float distToFocus = 10.0;
  float aperture = 0.0;

  Camera camera(lookfrom, lookat, vec3(0, 1, 0), 40, width / height, aperture, distToFocus);

  shared_ptr<BVH> bvh(new BVH(world.list));
  return Scene{bvh, camera};
}

Scene sceneWeek(int width, int height) {
  HitList world;

  shared_ptr<Material> white = make_shared<Lambertian>(make_shared<ConstantTexture>(0.73, 0.73, 0.73));
  shared_ptr<Material> ground = make_shared<Lambertian>(make_shared<ConstantTexture>(0.48, 0.83, 0.53));

  shared_ptr<HitList> boxlist = make_shared<HitList>();
  for (int i = 0; i < 20; ++i) {
    for (int j = 0; j < 20; ++j) {
      float w = 100;
      float x0 = -1000 + i * w;
      float z0 = -1000 + j * w;
      float y0 = 0;
      float x1 = x0 + w;
      float y1 = 100 * (frand() + 0.01);
      float z1 = z0 + w;
      boxlist->add(make_shared<Box>(vec3(x0, y0, z0), vec3(x1, y1, z1), ground));
    }
  }
  world.add(boxlist);

  shared_ptr<Material> light = make_shared<DiffuseLight>(make_shared<ConstantTexture>(7, 7, 7));
  world.add(make_shared<RectXZ>(123, 423, 147, 412, 554, light));

  vec3 center(400, 400, 200);

  world.add(make_shared<Sphere>(center, 50,
    make_shared<Lambertian>(make_shared<ConstantTexture>(0.7, 0.3, 0.1))));

  world.add(make_shared<Sphere>(vec3(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
  world.add(make_shared<Sphere>(vec3(0, 150, 145), 50,
    make_shared<Metal>(make_shared<ConstantTexture>(0.8, 0.8, 0.9), 10.0)));

  shared_ptr<Hitable> boundary = make_shared<Sphere>(vec3(360, 150, 145), 70,
    make_shared<Dielectric>(1.5));
  world.add(boundary);
  world.add(make_shared<ConstantMedium>(boundary, 0.2, make_shared<ConstantTexture>(0.2, 0.4, 0.9)));
  boundary = make_shared<Sphere>(vec3(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
  world.add(make_shared<ConstantMedium>(boundary, 0.0001, make_shared<ConstantTexture>(1.0, 1.0, 1.0)));

  shared_ptr<Texture> earth = make_shared<ImageTexture>("./textures/2k_earth_daymap.jpg");
  shared_ptr<Material> emat = make_shared<Lambertian>(earth);
  world.add(make_shared<Sphere>(vec3(400, 200, 400), 100, emat));

  shared_ptr<Texture> pertext = make_shared<NoiseTexture>(0.1);
  world.add(make_shared<Sphere>(vec3(220, 280, 300), 80, make_shared<Lambertian>(pertext)));


  shared_ptr<HitList> boxlist2 = make_shared<HitList>();
  for (int j = 0; j < 1000; ++j) {
    boxlist2->add(make_shared<Sphere>(vec3(
      165 * (frand() - 0.5), 165 * (frand() - 0.5), 165 * (frand() - 0.5)), 10, white));
  }

  mat4 bl2t;
  bl2t.translate(-100 + 82.5, 270 + 82.5, 395 + 82.5)
    .rotateXYZ(0, M_PI / 6, 0);

  world.add(make_shared<Transform>(bl2t, boxlist2));

  vec3 lookfrom(478, 278, -600);
  vec3 lookat(278, 278, 0);
  float distToFocus = 10.0;
  float aperture = 0.0;

  Camera camera(lookfrom, lookat, vec3(0, 1, 0), 30, width / height, aperture, distToFocus);

  shared_ptr<BVH> bvh(new BVH(world.list));
  return Scene{bvh, camera};
}

#endif
