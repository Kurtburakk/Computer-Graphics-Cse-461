#include "parser.hpp"
#include "raytracer.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <XML file path>" << std::endl;
        return 1;
    }

    std::string xml_file_path = argv[1];  // xml path with name

    parser::Scene scene;
    scene.loadFromXml(xml_file_path);
    std::string outputfile_name = scene.texture_image;

    parser::Camera &cam = scene.camera;
    int width = cam.image_width;
    int height = cam.image_height;
    unsigned char *image = new unsigned char[width * height * 3];

#pragma omp parallel for
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Ray ray = generateRay(cam, x, y);
            Hit hit = sendRayToObjects(scene.maxraytracedepth, scene, ray);

            // std::cout << "Pixel before hit [" << x << ", " << y<< "]: " << hit.pixel.x << " " << hit.pixel.y << " " << hit.pixel.z << std::endl;
            int r = std::min(std::max(hit.pixel.x, 0), 255);
            int g = std::min(std::max(hit.pixel.y, 0), 255);
            int b = std::min(std::max(hit.pixel.z, 0), 255);

            int index = (y * width + x) * 3;
            image[index] = static_cast<unsigned char>(r);
            image[index + 1] = static_cast<unsigned char>(g);
            image[index + 2] = static_cast<unsigned char>(b);
            std::cout << "Pixel [" << x << ", " << y << "]: " << r << " " << g << " " << b << std::endl;
        }
    }

    FILE *outfile = fopen(scene.texture_image.c_str(), "w");
    if (!outfile)
    {
        perror("Error opening file");
        delete[] image;
        throw std::runtime_error("Error: The ppm file cannot be opened for writing.");
    }

    fprintf(outfile, "P3\n%d %d\n255\n", width, height);
    for (size_t j = 0, idx = 0; j < height; ++j)
    {
        for (size_t i = 0; i < width; ++i)
        {
            for (size_t c = 0; c < 3; ++c, ++idx)
            {
                if (i == width - 1 && c == 2)
                {
                    fprintf(outfile, "%d", image[idx]);
                }
                else
                {
                    fprintf(outfile, "%d ", image[idx]);
                }
            }
        }
        fprintf(outfile, "\n");
    }
    fclose(outfile);

    delete[] image;

    /*
     *
     * PARSER TEST PART
     * 
     */
    std::cout << "Parser test..." << std::endl;
    try
    {

        // Background color
        std::cout << "Background Color: "
                  << scene.background_color.x << " "
                  << scene.background_color.y << " "
                  << scene.background_color.z << std::endl;

        // Max ray trace depth
        std::cout << "Max Ray Trace Depth: " << scene.maxraytracedepth << std::endl;

        // Camera 
        std::cout << "Camera Position: "
                  << scene.camera.position.x << " "
                  << scene.camera.position.y << " "
                  << scene.camera.position.z << std::endl;

        std::cout << "Camera Gaze: "
                  << scene.camera.gaze.x << " "
                  << scene.camera.gaze.y << " "
                  << scene.camera.gaze.z << std::endl;

        std::cout << "Camera Up: "
                  << scene.camera.up.x << " "
                  << scene.camera.up.y << " "
                  << scene.camera.up.z << std::endl;

        std::cout << "Camera Near Plane: "
                  << scene.camera.near_plane.x << " "
                  << scene.camera.near_plane.y << " "
                  << scene.camera.near_plane.z << " "
                  << scene.camera.near_plane.w << std::endl;

        std::cout << "Camera Near Distance: "
                  << scene.camera.near_distance << std::endl;

        std::cout << "Camera Resolution: "
                  << scene.camera.image_width << "x"
                  << scene.camera.image_height << std::endl;

        // Ambient light
        std::cout << "Ambient Light: "
                  << scene.ambient_light.x << " "
                  << scene.ambient_light.y << " "
                  << scene.ambient_light.z << std::endl;

        // Point light 
        std::cout << "Point Lights: " << std::endl;
        for (const auto &point_light : scene.point_lights)
        {
            std::cout << "id : " << point_light.id << std::endl;
            std::cout << "  Position: "
                      << point_light.position.x << " "
                      << point_light.position.y << " "
                      << point_light.position.z << std::endl;

            std::cout << "  Intensity: "
                      << point_light.intensity.x << " "
                      << point_light.intensity.y << " "
                      << point_light.intensity.z << std::endl;
        }

        // Triangular light 
        std::cout << "Triangular Lights: " << std::endl;
        for (const auto &triangular_light : scene.triangular_lights)
        {
            std::cout << "id : " << triangular_light.id << std::endl;
            std::cout << "  Vertex1: "
                      << triangular_light.vertex1.x << " "
                      << triangular_light.vertex1.y << " "
                      << triangular_light.vertex1.z << std::endl;

            std::cout << "  Vertex2: "
                      << triangular_light.vertex2.x << " "
                      << triangular_light.vertex2.y << " "
                      << triangular_light.vertex2.z << std::endl;

            std::cout << "  Vertex3: "
                      << triangular_light.vertex3.x << " "
                      << triangular_light.vertex3.y << " "
                      << triangular_light.vertex3.z << std::endl;

            std::cout << "  Intensity: "
                      << triangular_light.intensity.x << " "
                      << triangular_light.intensity.y << " "
                      << triangular_light.intensity.z << std::endl;
        }

        // Material 
        std::cout << "Materials: " << std::endl;
        for (const auto &material : scene.materials)
        {
            std::cout << "id : " << material.id << std::endl;
            std::cout << "  Ambient: "
                      << material.ambient.x << " "
                      << material.ambient.y << " "
                      << material.ambient.z << std::endl;

            std::cout << "  Diffuse: "
                      << material.diffuse.x << " "
                      << material.diffuse.y << " "
                      << material.diffuse.z << std::endl;

            std::cout << "  Specular: "
                      << material.specular.x << " "
                      << material.specular.y << " "
                      << material.specular.z << std::endl;

            std::cout << "  Mirror Reflectance: "
                      << material.mirror_reflactance.x << " "
                      << material.mirror_reflactance.y << " "
                      << material.mirror_reflactance.z << std::endl;

            std::cout << "  Phong Exponent: " << material.phong_exponent << std::endl;

            std::cout << "  Texture Factor: " << material.texture_factor << std::endl;
        }

        // Vertex data
        std::cout << "Vertex Data: " << std::endl;
        for (const auto &vertex : scene.vertex_data)
        {
            std::cout << "  " << vertex.x << " "
                      << vertex.y << " "
                      << vertex.z << std::endl;
        }

        // Texture data
        std::cout << "Texture Data: " << std::endl;
        for (const auto &texture : scene.texture_data)
        {
            std::cout << "  " << texture.x << " "
                      << texture.y << std::endl;
        }

        // Normal data
        std::cout << "Normal Data: " << std::endl;
        for (const auto &normal : scene.normal_data)
        {
            std::cout << "  " << normal.x << " "
                      << normal.y << " "
                      << normal.z << std::endl;
        }

        // Texture image
        std::cout << "Texture Image: " << scene.texture_image << std::endl;

        // Meshes
        std::cout << "Meshes: " << std::endl;
        for (const auto &mesh : scene.meshes)
        {

            std::cout << "  Mesh id: " << mesh.id << std::endl;
            std::cout << "  Material id: " << mesh.material_id << std::endl;

            std::cout << "  Faces: " << std::endl;
            for (const auto &face : mesh.faces)
            {
                std::cout << "    " << face.v1_id << " "
                          << face.t1_id << " "
                          << face.n1_id
                          << "    " << face.v2_id << " "
                          << face.t2_id << " "
                          << face.n2_id
                          << "    " << face.v3_id << " "
                          << face.t3_id << " "
                          << face.n3_id << std::endl;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading XML: " << e.what() << std::endl;
    }
    return 0;
}
