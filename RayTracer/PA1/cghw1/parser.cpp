#include "parser.hpp"
#include "tinyxml2.h"
#include <sstream>
#include <stdexcept>
#include <algorithm> 

void parser::Scene::loadFromXml(const std::string &filepath) {
    tinyxml2::XMLDocument file;
    std::stringstream stream;

    auto res = file.LoadFile(filepath.c_str());
    if (res) {
        throw std::runtime_error("Error: The xml file cannot be loaded.");
    }

    auto root = file.RootElement(); 
    if (!root) {
        throw std::runtime_error("Error: Root element not found.");
    }

    // Maxraytracedepth
    auto element = root->FirstChildElement("maxraytracedepth");
    if (element) {
        stream << element->GetText();
    } else {
        stream << "0";
    }
    stream >> maxraytracedepth;
    stream.clear();
    stream.str("");

    // BackgroundColor
    element = root->FirstChildElement("backgroundColor");
    if (element) {
        stream << element->GetText();
    } else {
        stream << "0 0 0";
    }
    stream >> background_color.x >> background_color.y >> background_color.z;
    stream.clear();
    stream.str("");

    // Camera
    element = root->FirstChildElement("camera");
    if (element) {
        auto child = element->FirstChildElement("position");
        if (child) stream << child->GetText() << " ";
        child = element->FirstChildElement("gaze");
        if (child) stream << child->GetText() << " ";
        child = element->FirstChildElement("up");
        if (child) stream << child->GetText() << " ";
        child = element->FirstChildElement("nearPlane");
        if (child) stream << child->GetText() << " ";
        child = element->FirstChildElement("neardistance");
        if (child) stream << child->GetText() << " ";
        child = element->FirstChildElement("imageresolution");
        if (child) stream << child->GetText() << " ";

        stream >> camera.position.x >> camera.position.y >> camera.position.z;
        stream >> camera.gaze.x >> camera.gaze.y >> camera.gaze.z;
        stream >> camera.up.x >> camera.up.y >> camera.up.z;
        stream >> camera.near_plane.x >> camera.near_plane.y >> camera.near_plane.z >> camera.near_plane.w;
        stream >> camera.near_distance;
        stream >> camera.image_width >> camera.image_height;
    }
    stream.clear();
    stream.str("");

    // Lights
    element = root->FirstChildElement("lights");
    if (element) {
        // AmbientLight
        auto child = element->FirstChildElement("ambientlight");
        if (child) {
            stream << child->GetText();
            stream >> ambient_light.x >> ambient_light.y >> ambient_light.z;
        }
        stream.clear();
        stream.str("");

        // PointLights
        auto pointElement = element->FirstChildElement("pointlight");
        while (pointElement) {
            PointLight point_light;
            point_light.id = pointElement->Attribute("id"); // id 
            child = pointElement->FirstChildElement("position");
            if (child) stream << child->GetText() << " ";
            child = pointElement->FirstChildElement("intensity");
            if (child) stream << child->GetText() << " ";

            stream >> point_light.position.x >> point_light.position.y >> point_light.position.z;
            stream >> point_light.intensity.x >> point_light.intensity.y >> point_light.intensity.z;

            point_lights.push_back(point_light);
            pointElement = pointElement->NextSiblingElement("pointlight");
            stream.clear();
            stream.str("");
        }

        // TriangularLights
        auto triElement = element->FirstChildElement("triangularlight");
        while (triElement) {
            TriangularLight tri_light;
            tri_light.id = triElement->Attribute("id"); // id 
            child = triElement->FirstChildElement("vertex1");
            if (child) stream << child->GetText() << " ";
            child = triElement->FirstChildElement("vertex2");
            if (child) stream << child->GetText() << " ";
            child = triElement->FirstChildElement("vertex3");
            if (child) stream << child->GetText() << " ";
            child = triElement->FirstChildElement("intensity");
            if (child) stream << child->GetText() << " ";

            stream >> tri_light.vertex1.x >> tri_light.vertex1.y >> tri_light.vertex1.z;
            stream >> tri_light.vertex2.x >> tri_light.vertex2.y >> tri_light.vertex2.z;
            stream >> tri_light.vertex3.x >> tri_light.vertex3.y >> tri_light.vertex3.z;
            stream >> tri_light.intensity.x >> tri_light.intensity.y >> tri_light.intensity.z;

            triangular_lights.push_back(tri_light);
            triElement = triElement->NextSiblingElement("triangularlight");
            stream.clear();
            stream.str("");
        }
    }

    // Materials
    element = root->FirstChildElement("materials");
    if (element) {
        auto matElement = element->FirstChildElement("material");
        while (matElement) {
            Material material;
            material.id = matElement->Attribute("id");  // id 
            auto child = matElement->FirstChildElement("ambient");
            if (child) stream << child->GetText() << " ";
            child = matElement->FirstChildElement("diffuse");
            if (child) stream << child->GetText() << " ";
            child = matElement->FirstChildElement("specular");
            if (child) stream << child->GetText() << " ";
            child = matElement->FirstChildElement("mirrorreflactance"); 
            if (child) stream << child->GetText() << " ";
            child = matElement->FirstChildElement("phongexponent");
            if (child) stream << child->GetText() << " ";
            child = matElement->FirstChildElement("texturefactor");
            if (child) stream << child->GetText() << " ";

            stream >> material.ambient.x >> material.ambient.y >> material.ambient.z;
            stream >> material.diffuse.x >> material.diffuse.y >> material.diffuse.z;
            stream >> material.specular.x >> material.specular.y >> material.specular.z;
            stream >> material.mirror_reflactance.x >> material.mirror_reflactance.y >> material.mirror_reflactance.z;
            stream >> material.phong_exponent;
            stream >> material.texture_factor;

            materials.push_back(material);
            matElement = matElement->NextSiblingElement("material");
            stream.clear();
            stream.str("");
        }
    }

    // Vertex Data
    element = root->FirstChildElement("vertexdata");
    if (element) {
        stream << element->GetText();
        Vec3f vertex;
        while (stream >> vertex.x >> vertex.y >> vertex.z) {
            vertex_data.push_back(vertex);
        }
        stream.clear();
        stream.str("");
    }

    // Texture Data
    element = root->FirstChildElement("texturedata");
    if (element) {
        stream << element->GetText();
        Vec3f texture;
        while (stream >> texture.x >> texture.y) {
            texture.z = 0.0f;
            texture_data.push_back(texture);
        }
        stream.clear();
        stream.str("");
    }

    // Normal Data
    element = root->FirstChildElement("normaldata");
    if (element) {
        stream << element->GetText();
        Vec3f normal;
        while (stream >> normal.x >> normal.y >> normal.z) {
            normal_data.push_back(normal);
        }
        stream.clear();
        stream.str("");
    }

    // Texture Image
    element = root->FirstChildElement("textureimage");
    if (element) {
        texture_image = element->GetText();
    }

    // Meshes
    element = root->FirstChildElement("objects");
    if (element) {
        auto meshElement = element->FirstChildElement("mesh");
        while (meshElement) {
            Mesh mesh;
            mesh.id = meshElement->Attribute("id"); // id 
            auto child = meshElement->FirstChildElement("materialid");
            if (child) {
                stream << child->GetText();
                stream >> mesh.material_id;
            }
            stream.clear();
            stream.str("");

            child = meshElement->FirstChildElement("faces");
            if (child) {
                std::string facesText = child->GetText();
                std::replace(facesText.begin(), facesText.end(), '\n', ' ');
                std::stringstream faceStream(facesText);
                std::string token;

                while (faceStream >> token) {
                    Face face;
                    for (int i = 0; i < 3; ++i) { 
                        std::replace(token.begin(), token.end(), '/', ' ');
                        std::stringstream tokenStream(token);
                        int v, t, n;
                        tokenStream >> v >> t >> n;

                        if (i == 0) {
                            face.v1_id = v;
                            face.t1_id = t;
                            face.n1_id = n;
                        } else if (i == 1) {
                            face.v2_id = v;
                            face.t2_id = t;
                            face.n2_id = n;
                        } else if (i == 2) {
                            face.v3_id = v;
                            face.t3_id = t;
                            face.n3_id = n;
                        }

                        if (i < 2) faceStream >> token; 
                    }
                    mesh.faces.push_back(face);
                }
            }
            meshes.push_back(mesh);
            meshElement = meshElement->NextSiblingElement("mesh");
        }
    }
}