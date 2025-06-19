#include <iostream>
#include "parser.hpp"
#include "raytracer.hpp"
#include <cmath>
#include <vector>
#include <limits>
#include <chrono>
using namespace std;
using namespace parser;


bool DEBUG = false;   // debug check intersection


Vec3f crossProduct(const Vec3f &a, const Vec3f &b)
{
    return Vec3f{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x};
}

float dotProduct(const Vec3f &a, const Vec3f &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float magnitude(const Vec3f &v)
{
    return sqrt(dotProduct(v, v));
}

Vec3f normalize(const Vec3f &v)
{
    Vec3f result = v;
    double length = magnitude(v);
    if (length == 0)
        return result;
    result.x /= length;
    result.y /= length;
    result.z /= length;
    return result;
}

Ray generateRay(const Camera &cam, int i, int j) {
    float l = cam.near_plane.x;
    float r = cam.near_plane.y;
    float b = cam.near_plane.z;
    float t = cam.near_plane.w;

    // (m) = camera position + gaze * near_distance
    Vec3f m = cam.position + cam.gaze * cam.near_distance;

    Vec3f u = normalize(crossProduct(cam.up, cam.gaze * -1));   
    Vec3f v = normalize(crossProduct(cam.gaze * -1, u));        

    Vec3f q = m + u * l + v * t;

    double su = (i + 0.5) * (r - l) / cam.image_width;
    double sv = (j + 0.5) * (t - b) / cam.image_height;


    Vec3f s = q + u * su - v * sv;   

    Ray ray;
    ray.origin = cam.position;
    ray.direction = normalize(s - cam.position);

    if (DEBUG) {
        std::cout << "[DEBUG] generateRay: pixel (" << i << ", " << j 
                  << ") -> ray.direction = (" 
                  << ray.direction.x << ", " << ray.direction.y << ", " 
                  << ray.direction.z << ")" << std::endl;
    }
    return ray;
}


float intersectionPointLight(Scene const &scene, Ray const &ray, Vec3f center)
{
    double radius = pow(10, -1);

    float A = dotProduct(ray.direction, ray.direction);
    float B = 2 * dotProduct(ray.direction, ray.origin - center);
    float C = dotProduct(ray.origin - center, ray.origin - center) - pow(radius, 2);

    float discriminant = pow(B, 2) - 4 * A * C;

    if (discriminant < 0)
        return -1;
    else
    {
        float t1 = (-1 * B + sqrtf(discriminant)) / (2 * A);
        float t2 = (-1 * B - sqrtf(discriminant)) / (2 * A);
        float min = t1 < t2 ? t1 : t2;
        if (DEBUG)
            std::cout << "[DEBUG] intersectionPointLight: t = " << min << std::endl;
        return min;
    }
}
float intersectionTriangle(const Scene &scene, const Ray &ray, const Face &face) {
    const float EPSILON = 1e-6;

    Vec3f v0 = scene.vertex_data[face.v1_id - 1];
    Vec3f v1 = scene.vertex_data[face.v2_id - 1];
    Vec3f v2 = scene.vertex_data[face.v3_id - 1];


    Vec3f edge1 = v1 - v0;
    Vec3f edge2 = v2 - v0;

    Vec3f h = crossProduct(ray.direction, edge2);
    float a = dotProduct(edge1, h);
    if (fabs(a) < EPSILON) {
        return -1;  
    }

    float f = 1.0f / a;
    Vec3f s = ray.origin - v0;
    float u = f * dotProduct(s, h);
    if (u < 0.0f || u > 1.0f) {
        return -1;  
    }

    Vec3f q = crossProduct(s, edge1);
    float v = f * dotProduct(ray.direction, q);
    if (v < 0.0f || (u + v) > 1.0f) {
        return -1;  
    }

    float t = f * dotProduct(edge2, q);
    if (t > EPSILON) {
        return t;   
    } else {
        return -1;  
    }
}



Vec3f calculateIrradience(Hit hit, PointLight pointLight)
{
    Vec3f irradience;
    Vec3f value_before_length = pointLight.position - hit.intersectionPoint;
    Vec3f tmp = value_before_length * value_before_length;
    float r = tmp.x + tmp.y + tmp.z;

    if (r != 0)
    {
        irradience.x = pointLight.intensity.x / r;
        irradience.y = pointLight.intensity.y / r;
        irradience.z = pointLight.intensity.z / r;
    }
    return irradience;
}

Vec3f calculateDiffuse(Hit hit, PointLight pointLight, Vec3f irradiance)
{
    Vec3f W_i = pointLight.position - hit.intersectionPoint;
    W_i = normalize(W_i);
    double cos_teta = dotProduct(hit.normal, W_i);
    double epsilon = pow(10, -6);
    if (cos_teta < 0 + epsilon)
        cos_teta = 0;

    return hit.material.diffuse * (irradiance * cos_teta);
}

Vec3f calculateSpecular(Hit hit, Ray ray, Vec3f irradiance, PointLight pointLight)
{
    Vec3f W_i = pointLight.position - hit.intersectionPoint;
    W_i = normalize(W_i);
    Vec3f W_o = ray.origin - hit.intersectionPoint;
    W_o = normalize(W_o);
    Vec3f h = W_i + W_o;
    h = normalize(h);

    double cos_alpha_teta = dotProduct(hit.normal, h);
    double epsilon = pow(10, -6);
    if (cos_alpha_teta < 0 + epsilon)
        cos_alpha_teta = 0;
    double tmp = pow(cos_alpha_teta, hit.material.phong_exponent);

    return hit.material.specular * (irradiance * tmp);
}

int detectShadow(const Scene &scene, const PointLight &pointLight, const Vec3f &intersectionPoint, const Hit &hit)
{
    Ray shadow;
    shadow.direction = pointLight.position - intersectionPoint;
    shadow.direction = normalize(shadow.direction);
    shadow.origin = intersectionPoint + hit.normal * SHADOW_RAY_EPSILON;

    float lightDistance = magnitude(pointLight.position - shadow.origin);
    if (DEBUG)
        std::cout << "[DEBUG] detectShadow: lightDistance = " << lightDistance << std::endl;

    for (const Mesh &mesh : scene.meshes)
    {
        for (const Face &face : mesh.faces)
        {
            float t = intersectionTriangle(scene, shadow, face);
            if (DEBUG)
                std::cout << "[DEBUG] detectShadow: t for a face = " << t << std::endl;
            if (t >= 0 && t <= lightDistance)
            {
                if (DEBUG)
                    std::cout << "[DEBUG] detectShadow: Shadow detected!" << std::endl;
                return 1; // shadow
            }
        }
    }
    return -1; // not shadow
}

Ray detectMirror(Scene const &scene, Ray const &ray, Hit const &hit)
{
    Ray result;
    Vec3f w0_direction = ray.direction * -1;
    float dp = dotProduct(hit.normal, w0_direction);
    Vec3f w_r_direction = ray.direction + (hit.normal * 2) * dp;
    w_r_direction = normalize(w_r_direction);
    result.origin = hit.intersectionPoint + hit.normal * SHADOW_RAY_EPSILON;
    result.direction = w_r_direction;
    return result;
}
Hit sendRayToObjects(int recursion_number, const Scene &scene, const Ray &ray) {
    Hit hit;
    hit.pixel = scene.background_color; 
    float t = -1;
    int hitMeshIndex = -1;
    int hitFaceIndex = -1;

    for (int i = 0; i < scene.meshes.size(); ++i) {
        const Mesh& mesh = scene.meshes[i];
        for (int j = 0; j < mesh.faces.size(); ++j) {
            float tmp_t = intersectionTriangle(scene, ray, mesh.faces[j]);
            if (DEBUG) {
                std::cout << "[DEBUG] sendRayToObjects: Mesh " << i 
                          << ", Face " << j << ", t = " << tmp_t << std::endl;
            }
            if (tmp_t > 0) {  // find intersection
                if (t < 0 || tmp_t < t) {
                    // find more near intersection
                    t = tmp_t;
                    hitMeshIndex = i;
                    hitFaceIndex = j;
                }
            }
        }
    }

    if (t < 0) {
        if (recursion_number > 0) {
            hit.pixel = scene.background_color;
            if (DEBUG) {
                std::cout << "[DEBUG] sendRayToObjects: No hit, returning background." << std::endl;
            }
        }
        return hit;
    }

    const Mesh& hitMesh = scene.meshes[hitMeshIndex];
    const Face& face = hitMesh.faces[hitFaceIndex];
    hit.material = scene.materials[hitMesh.material_id - 1];

    Vec3f t1 = scene.texture_data[face.t1_id - 1];
    Vec3f t2 = scene.texture_data[face.t2_id - 1];
    Vec3f t3 = scene.texture_data[face.t3_id - 1];
    Vec3f uv = (t1 + t2 + t3) / 3.0f;
    
    hit.material.diffuse = hit.material.diffuse * (1 - hit.material.texture_factor) 
                           + uv * hit.material.texture_factor;

    
    hit.intersectionPoint = ray.origin + ray.direction * t;
    if (DEBUG) {
        std::cout << "[DEBUG] Intersection at (" 
                  << hit.intersectionPoint.x << ", " 
                  << hit.intersectionPoint.y << ", " 
                  << hit.intersectionPoint.z << ")" << std::endl;
    }

    
    Vec3f n1 = scene.normal_data[face.n1_id - 1];
    Vec3f n2 = scene.normal_data[face.n2_id - 1];
    Vec3f n3 = scene.normal_data[face.n3_id - 1];
    hit.normal = normalize(n1 + n2 + n3);
    
    if (dotProduct(ray.direction, hit.normal) > 0) {
        hit.normal = hit.normal * -1;
    }

    
    Vec3f color = {0, 0, 0};

    
    color = color + hit.material.ambient * scene.ambient_light;
    if (DEBUG) {
        std::cout << "[DEBUG] Ambient color contribution = (" 
                  << color.x << ", " << color.y << ", " << color.z << ")" << std::endl;
    }

    
    for (const PointLight& pointLight : scene.point_lights) {
    
        int shadow = detectShadow(scene, pointLight, hit.intersectionPoint, hit);
        if (DEBUG) {
            std::cout << "[DEBUG] Shadow check = " << shadow << std::endl;
        }
        if (shadow != 1) {  
    
            Vec3f irradiance = calculateIrradience(hit, pointLight);
    

    
            Vec3f L = normalize(pointLight.position - hit.intersectionPoint);
            float cosTheta = dotProduct(hit.normal, L);
            if (cosTheta < 0) cosTheta = 0;  
            Vec3f diffuse = hit.material.diffuse * (irradiance * cosTheta);

    
            Vec3f V = normalize(ray.origin - hit.intersectionPoint);         
            Vec3f H = normalize(L + V);                                     
            float cosAlpha = dotProduct(hit.normal, H);
            if (cosAlpha < 0) cosAlpha = 0;
            float specFactor = pow(cosAlpha, hit.material.phong_exponent);
            Vec3f specular = hit.material.specular * (irradiance * specFactor);

            if (DEBUG) {
                std::cout << "[DEBUG] Diffuse = (" 
                          << diffuse.x << ", " << diffuse.y << ", " << diffuse.z << "), "
                          << "Specular = (" 
                          << specular.x << ", " << specular.y << ", " << specular.z << ")" 
                          << std::endl;
            }
            color = color + diffuse + specular;
        }
    }


    if ((hit.material.mirror_reflactance.x > 0 || hit.material.mirror_reflactance.y > 0 ||
         hit.material.mirror_reflactance.z > 0) && recursion_number < scene.maxraytracedepth) {
        if (DEBUG) std::cout << "[DEBUG] Calculating mirror reflection...\n";
        Ray mirrorRay = detectMirror(scene, ray, hit);
        Hit mirrorHit = sendRayToObjects(recursion_number + 1, scene, mirrorRay);

        color = color + mirrorHit.pixel * hit.material.mirror_reflactance;
    }


    color.x = std::min(std::max(color.x, 0.0f), 255.0f);
    color.y = std::min(std::max(color.y, 0.0f), 255.0f);
    color.z = std::min(std::max(color.z, 0.0f), 255.0f);


    hit.pixel.x = static_cast<int>(color.x);
    hit.pixel.y = static_cast<int>(color.y);
    hit.pixel.z = static_cast<int>(color.z);

    if (DEBUG) {
        std::cout << "[DEBUG] Final pixel color = (" 
                  << hit.pixel.x << ", " << hit.pixel.y << ", " << hit.pixel.z << ")" << std::endl;
    }

    return hit;
}
