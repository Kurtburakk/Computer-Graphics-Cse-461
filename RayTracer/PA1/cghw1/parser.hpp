#ifndef __HW1__PARSER__
#define __HW1__PARSER__

#include <string>
#include <vector>

namespace parser
{
    struct Vec3f
    {
        float x, y, z;
    };

    struct Vec3i
    {
        int x, y, z;
    };

    struct Vec4f
    {
        float x, y, z, w;
    };

    inline Vec3f operator +(const Vec3f& vec1,const Vec3f& vec2){
        Vec3f tmpVec;
        tmpVec.x = vec1.x+vec2.x;
        tmpVec.y = vec1.y+vec2.y;
        tmpVec.z = vec1.z+vec2.z;

        return tmpVec;
    }

    inline Vec3f operator -(const Vec3f& vec1,const Vec3f& vec2){
        Vec3f tmpVec;
        tmpVec.x = vec1.x-vec2.x;
        tmpVec.y = vec1.y-vec2.y;
        tmpVec.z = vec1.z-vec2.z;

        return tmpVec;
    }
    inline Vec3f operator -(const Vec3f& vec) {
        return Vec3f{-vec.x, -vec.y, -vec.z};
    }

    inline Vec3f operator *(const Vec3f& vec1,const float &number){
        Vec3f tmpVec;
        tmpVec.x = vec1.x*number;
        tmpVec.y = vec1.y*number;
        tmpVec.z = vec1.z*number;

        return tmpVec;
    }

    inline Vec3f operator *(const Vec3f& vec1,const Vec3f &vec2){
        Vec3f tmpVec;
        tmpVec.x = vec1.x*vec2.x;
        tmpVec.y = vec1.y*vec2.y;
        tmpVec.z = vec1.z*vec2.z;

        return tmpVec;
    }

    inline Vec3f operator /(const Vec3f& vec1,const float &number){
        Vec3f tmpVec;
        tmpVec.x = vec1.x/number;
        tmpVec.y = vec1.y/number;
        tmpVec.z = vec1.z/number;

        return tmpVec;
    }

    inline Vec3f operator *(const Vec3i& vec1,const Vec3f &vec2){
        Vec3f tmpVec;
        tmpVec.x = vec1.x*vec2.x;
        tmpVec.y = vec1.y*vec2.y;
        tmpVec.z = vec1.z*vec2.z;

        return tmpVec;
    }

    struct Camera
    {
        Vec3f position;
        Vec3f gaze;
        Vec3f up;
        Vec4f near_plane;
        float near_distance;
        int image_width, image_height;
    };

    struct PointLight
    {
        std::string id;
        Vec3f position;
        Vec3f intensity;
    };
    
    struct TriangularLight
    {
        std::string id;
        Vec3f vertex1;
        Vec3f vertex2;
        Vec3f vertex3;
        Vec3f intensity;
    };
    

    struct Material
    {
        std::string id;
        Vec3f ambient;
        Vec3f diffuse;
        Vec3f specular;
        Vec3f mirror_reflactance;
        float phong_exponent;
        float texture_factor;
    };

    struct Face
    {
        int v1_id ,t1_id , n1_id ;
        int v2_id ,t2_id , n2_id ;
        int v3_id ,t3_id , n3_id ;
    };
    

    struct Mesh {
        std::string id; 
        int material_id;
        std::vector<Face> faces;
    };

    struct Scene
    {
        int maxraytracedepth;      
        Vec3i background_color;
        Camera camera;
        Vec3f ambient_light;
        std::vector<PointLight> point_lights;
        std::vector<TriangularLight> triangular_lights;
        std::vector<Material> materials;
        std::vector<Vec3f> vertex_data;
        std::vector<Vec3f> texture_data;
        std::vector<Vec3f> normal_data;
        std::string texture_image;
        std::vector<Mesh> meshes;

        void loadFromXml(const std::string &filepath);
    };
}

#endif
