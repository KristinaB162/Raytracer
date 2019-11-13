//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================


//== includes =================================================================

#include "StopWatch.h"
#include "Sphere.h"
#include "Plane.h"
#include "Mesh.h"
#include "Light.h"
#include "Ray.h"
#include "Material.h"
#include "Image.h"
#include "Camera.h"
#include <lodepng.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <float.h>


/// \file raytrace.cpp
/// In this file the main raytracing routines are implemented.


//== function declarations ====================================================

/// reads the scene to raytrace from the given `_filename`
void  read_scene(const char* _filename);

/// main function where the image is allocated and the raytracing is started
void  compute_image();

/// Finds the closest colision point for the passed Ray and returns the computed color for this object.
/**
*	@param _ray passed Ray
*	@param _depth holds the information, how many times the `_ray` had been reflected. Goes from 0 to max_depth. Should be used for recursive function call.
**/
vec3  trace(const Ray& _ray, int _depth);

/// Computes the closest intersection point between a ray and all objects in the scene.
/**
*   	@param _ray Ray that should be tested for intersections with all objects in the scene.
*   	@param _intersection_material returns material of intersected object.
*   	@param _intersection_point returns intersection point
*   	@param _intersection_normal returns normal at `_point`
*   	@param _t returns distance between the `_ray`'s origin and `_point`
*   	@return returns `true`, if there is an intersection point between `_ray` and at least one object in the scene.
**/
bool  intersect_scene(const Ray& _ray, Material &_intersection_material, vec3& _intersection_point, vec3& _intersection_normal, double& _t);

/// Computes the phong lighting.
/**
*	@param _point the point, whose color should be determined.
*	@param _normal `_point`'s normal
*	@param _view normalized direction from the `_point' to the viewer's position.
* 	@param _material holds material parameters of the `_point`, that should be lit.
*/
vec3  lighting(const vec3& _point, const vec3& _normal, const vec3& _view, const Material& _material);


//== global variables =========================================================


/// camera stores eye position, view direction, and can generate primary rays
Camera camera;

/// our image is just a large array of color values
Image image;

/// array for all lights in the scene
std::vector<Light> lights;

/// array for all the objects in the scene
std::vector<Object_ptr> objects;

/// max recursion depth for mirroring
int max_depth;

/// background color
vec3 background;

/// global ambient light
vec3 ambience;


//== implementation ===========================================================


int main(int argc, char **argv)
{

    if (argc < 2 || argc > 3)
    {
        std::cerr << "Usage: " << argv[0] << "  input.sce output.png\n\n" << std::endl;
        exit(1);
    }

    if(argc == 3)
    {
        // read scene from file
        std::cout << "Read scene...";
        read_scene(argv[1]);
        std::cout << "\ndone (" << objects.size() << " objects)\n";


        // raytrace image
        StopWatch timer;
        timer.start();
        std::cout << "Ray tracing..." << std::flush;

        compute_image();
        timer.stop();
        std::cout << "done (" << timer << ")\n";


        // write the resulting image
        std::cout << "Write image...";
        image.write_png(argv[2]);
        std::cout << "done\n";


        // clean up
        for (Object_ptr o: objects)
        {
            delete o;
        }
    }

    if (argc == 2)
    {
        if(argv[1][0] != '0')
        {
            std::cerr << "Usage: " << argv[0] << "  input.sce output.png\n\n" << std::endl;
            exit(1);
        }

        std::cout << "rendering all scenes..." << std::endl;

        for(int i = 0; i <  10; i++)
        {
            std::string si, so;
            switch(i)
            {
#ifdef _WIN32
                case 0: si = "../../scenes/spheres/spheres.sce"; so = "o_01_spheres.png"; break;
                case 1: si = "../../scenes/molecule/molecule.sce"; so = "o_02_molecule.png"; break;
                case 2: si = "../../scenes/molecule2/molecule2.sce"; so = "o_03_molecule2.png"; break;
                case 3: si = "../../scenes/cube/cube.sce"; so = "o_04_cube.png"; break;
                case 4: si = "../../scenes/mask/mask.sce"; so = "o_05_mask.png"; break;
                case 5: si = "../../scenes/mirror/mirror.sce"; so = "o_06_mirror.png"; break;
                case 6: si = "../../scenes/toon_faces/toon_faces.sce"; so = "o_07_toon_faces.png"; break;
                case 7: si = "../../scenes/office/office.sce"; so = "o_08_office.png"; break;
                case 8: si = "../../scenes/rings/rings.sce"; so = "o_09_rings.png"; break;
                case 9: si = "../../scenes/pokemon/pokemon.sce"; so = "o_10_pokemon.png"; break;
#else
                case 0: si = "../scenes/spheres/spheres.sce"; so = "o_01_spheres.png"; break;
                case 1: si = "../scenes/molecule/molecule.sce"; so = "o_02_molecule.png"; break;
                case 2: si = "../scenes/molecule2/molecule2.sce"; so = "o_03_molecule2.png"; break;
                case 3: si = "../scenes/cube/cube.sce"; so = "o_04_cube.png"; break;
                case 4: si = "../scenes/mask/mask.sce"; so = "o_05_mask.png"; break;
                case 5: si = "../scenes/mirror/mirror.sce"; so = "o_06_mirror.png"; break;
                case 6: si = "../scenes/toon_faces/toon_faces.sce"; so = "o_07_toon_faces.png"; break;
                case 7: si = "../scenes/office/office.sce"; so = "o_08_office.png"; break;
                case 8: si = "../scenes/rings/rings.sce"; so = "o_09_rings.png"; break;
                case 9: si = "../scenes/pokemon/pokemon.sce"; so = "o_10_pokemon.png"; break;
#endif
            }

            // read scene from file
            std::cout << "Read scene...";
            read_scene(si.c_str());
            std::cout << "\ndone (" << objects.size() << " objects)\n";


            // raytrace image
            StopWatch timer;
            timer.start();
            std::cout << "Ray tracing..." << std::flush;
            compute_image();
            timer.stop();
            std::cout << "done (" << timer << ")\n";


            // write the resulting image
            std::cout << "Write image..." << std::flush;
            image.write_png(so.c_str());
            std::cout << "done\n";


            // clean up
            for (Object_ptr o: objects)
            {
                delete o;
            }
            lights.resize(0);
            objects.resize(0);
        }
    }
}


//-----------------------------------------------------------------------------


void read_scene(const char* _filename)
{
    std::ifstream ifs(_filename);
    if (!ifs) {
        std::cerr << "Cannot open file " << _filename << std::endl;
        exit(1);
    }


    char line[200];
    std::string token;


    // parse file
    while (ifs && (ifs >> token) && (!ifs.eof())) {
        if (token[0] == '#') {
            ifs.getline(line, 200);
        } else if (token == "depth") {
            ifs >> max_depth;
        } else if (token == "camera") {
            ifs >> camera;
        } else if (token == "background") {
            ifs >> background;
        } else if (token == "ambience") {
            ifs >> ambience;
        } else if (token == "light") {
            Light light;
            ifs >> light;
            lights.push_back(light);
        } else if (token == "plane") {
            Plane *p = new Plane;
            ifs >> (*p);
            objects.push_back(p);
        } else if (token == "sphere") {
            Sphere *sphere = new Sphere();
            ifs >> (*sphere);
            objects.push_back(sphere);
        } else if (token == "mesh") {
            std::string filename, mode;
            ifs >> filename >> mode;

            // add path of scene-file to mesh's filename
            std::string path(_filename);
            path = path.substr(0, path.find_last_of('/') + 1);
            filename = path + filename;

            Mesh *mesh = new Mesh((mode == "FLAT" ? Mesh::FLAT : Mesh::PHONG),
                                  filename);

            ifs >> mesh->material;


            objects.push_back(mesh);
            }
        }
    ifs.close();
}


//-----------------------------------------------------------------------------


void compute_image() {


    // allocate memory by resizing image
    image.resize(camera.width, camera.height);

    /** \todo
     * (BONUS TASK) Implement supersampling to avoid aliasing artifacts:
     * - Instead of casting just one, you cast n*n rays per pixel and store the average of the traced color values.
     * - Start with 2x2 rays per pixel and check the result with the cube scene.
     * - Try to generalize this to support arbitrary n*n supersampling
     * - To cast this many rays will slow down your image computation. To avoid this, implement adaptive supersampling:
     *      * Supersampling is just needed if neighboring pixels have a noticable difference in their color values
     *      * Start with one ray per pixel and store the result in a temporary `Image` variable
     *      * Now, iterate a second time over your image and compare the color values of the each pixel with those of its neighbors' pixels
     *      * If this difference is higher than a certain threshold, you apply 4x4 supersampling (SSAA16) for this pixel
     *      * Experiment with the rings scene to find a good threshold
     * Hints:
     * - Some image viewers will blur your images by default to avoid aliasing if you zoom in,
     * open your images with an image manipulation tool instead.
     * - It may help to visualize the x and y coordinates of your subrays on a sheet of paper
     * - Try to avoid sqrt computations like in norm when you want to compute a color difference, use normSq instead
     */


// uncomment the following line to use multithreading
    #pragma omp parallel for
    for (int x=0; x<camera.width; ++x)
    {
        for (int y=0; y<camera.height; ++y)
        {
            Ray ray = camera.primary_ray(x,y);
            vec3 color = trace(ray,0);

            // avoid over-saturation
            color = min(color, vec3(1, 1, 1));

            // store pixel color
            image(x,y) = color;
        }
    }


}


//-----------------------------------------------------------------------------


vec3 trace(const Ray &_ray, int _depth) {
    // stop if recursion depth (=number of reflection) is too large
    if (_depth > max_depth) return vec3(0, 0, 0);

    // Find first intersection with an object. If an intersection is found,
    // it is stored in object, point, normal, and t.
    Material material;
    vec3 point;
    vec3 normal;
    double t;
    if (!intersect_scene(_ray, material, point, normal, t)){
        return background;
    }

    // compute local Phong lighting (ambient+diffuse+specular)

    vec3 color = lighting(point, normal, -_ray.direction, material);

    //std::cout << color << std::endl;

    /** \todo
     * Compute reflections by recursive ray tracing:
     * - check whether `object` is reflective by checking its `material.mirror`
     * - check recursion depth
     * - generate reflected ray, compute its color contribution, and mix it with
     * the color computed by local Phong lighthing (use `object->material.mirror` as weight)
     * - check whether your recursive algorithm reflects the ray `max_depth` times
     */
    
    if (material.mirror > 0) {
        // create reflection ray by mirroring it at normal
        vec3 reflection_direction = mirror(-_ray.direction, normal);
        Ray reflection_ray = Ray(point, reflection_direction);
        _depth += 1;
        vec3 new_color = trace(reflection_ray, _depth);
        color = (1-material.mirror) * color + (new_color * material.mirror);
    }
    

    return color;
}


//-----------------------------------------------------------------------------


bool intersect_scene(const Ray& _ray, Material &_intersection_material, vec3& _intersection_point, vec3& _intersection_normal, double& _t) {
    double t, tmin(DBL_MAX);
    vec3 p, n, d;

    for (Object_ptr o: objects) // for each object
    {
        if (o->intersect(_ray, p, n, d, t)) // does ray intersect object?
        {
            if (t < tmin) // is intersection point the currently closest one?
            {
                tmin = t;
                _intersection_material = o->material;
                _intersection_material.diffuse = d;
                _intersection_point = p;
                _intersection_normal = n;
                _t = t;
            }
        }
    }

    return (tmin < DBL_MAX);
}


//-----------------------------------------------------------------------------


vec3 lighting(const vec3 &_point, const vec3 &_normal, const vec3 &_view, const Material &_material) {
    vec3 color(0.0, 0.0, 0.0);

    /** \todo
    * Compute the Phong lighting:
    * x start with global ambient contribution
    * x for each light source (stored in vector `lights`) add diffuse and specular contribution
    * x only add diffuse and specular light if object is not in shadow
    *
    * Hints:
    * - All object specific material parameters (material's diffuse, specular, shininess) can be found in `_material`.
    * - The ambient material parameter is globally defined as `ambience`.
    * - Use the lights' member `color` for both diffuse and specular light intensity.
    * - Feel free to use the existing vector functions in vec3.h e.g. mirror, reflect, norm, dot, normalize
    */

    color = ambience * _material.ambient;

    //light.color = intensity
    for (auto light: lights) {
        

        vec3 light_vector;
        light_vector = light.position - _point;
        light_vector = normalize(light_vector);

        //shadows    
        Ray shadow_ray = Ray(_point, light_vector);
        Material _intersection_material;
        vec3 _intersection_point, _intersection_normal;
        double _t;
        double light_distance = distance(light.position, _point);
        bool intersection = intersect_scene(shadow_ray, _intersection_material, _intersection_point, _intersection_normal, _t);

        if (!(intersection == true && _t < light_distance) || _material.shadowable == false) {
            //compute diffuse/specular light ONLY IF no intersection and intersection point is not behind object
            
            //diffuse lighting
            vec3 diff = vec3();
            double diff_angle = dot(light_vector, _normal);
            if(diff_angle >= 0) {
                diff = light.color * _material.diffuse * diff_angle;
                color += diff;
            }
            //specular lighting
            vec3 spec = vec3();
            vec3 mirrored_light = vec3();
            mirrored_light = mirror(light_vector, _normal);
            double spec_angle = dot(mirrored_light, _view);
            if(diff_angle > 0 && spec_angle > 0) {
                spec = light.color * _material.specular * pow(spec_angle, _material.shininess);
                color += spec;
            }
        }


    }
    return color;
}

//=============================================================================
