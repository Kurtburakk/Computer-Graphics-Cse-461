#ifndef RAYTRACER_HPP
#define RAYTRACER_HPP

#include "parser.hpp"
#include <limits>


const float SHADOW_RAY_EPSILON = 1e-4;
const float INF = std::numeric_limits<float>::max();

struct Ray {
    parser::Vec3f origin;
    parser::Vec3f direction;
};

struct Hit {
    bool isHit = false;
    float t = INF;
    parser::Vec3f intersectionPoint;
    parser::Vec3f normal;
    parser::Vec3i pixel;
    parser::Material material;
};

parser::Vec3f crossProduct(const parser::Vec3f &a, const parser::Vec3f &b);
float dotProduct(const parser::Vec3f &a, const parser::Vec3f &b);
parser::Vec3f normalize(const parser::Vec3f &v);
Ray generateRay(const parser::Camera &cam, int i, int j);
float intersectionPointLight(parser::Scene const &scene, Ray const &ray, parser::Vec3f center);
float intersectionTriangle(const parser::Scene &scene, const Ray &ray, const parser::Face &face);
parser::Vec3f calculateIrradience(Hit hit, parser::PointLight pointLight);
parser::Vec3f calculateDiffuse(Hit hit, parser::PointLight pointLight, parser::Vec3f irradiance);
int detectShadow(const parser::Scene &scene, const parser::PointLight &pointLight, const parser::Vec3f &intersectionPoint, const Hit &hit);
Ray detectMirror(parser::Scene const &scene, Ray const &ray, Hit const &hit);
Hit sendRayToObjects(int recursion_number, parser::Scene const &scene, Ray const &ray);

#endif