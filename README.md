Ray Tracer
==========

We are using a couple of C++11 features, so please ensure that you use an up-to-date compiler (GCC 4.7 or higher, Visual Studio 2012). [CMake](www.cmake.org) is used for setting up build environments.

Building under Linux
--------------------

Inside the exercise's top-level directory, execute the following commands:

    mkdir build
    cd build
    cmake ..
    make

The last command -- i.e. `make` -- compiles the application. Rerun it whenever you have added/changed code in order to recompile.

Building under MacOS
--------------------

Inside the exercise's top-level directory, execute the following commands:

    mkdir xcode
    cd xcode
    cmake -G Xcode ..
    open RayTracing.xcodeproj



Running the Ray Tracer unter MaxOS and Linux
--------------------------------------------

The program expects two command line arguments: an input scene (`*.sce`) and an output image (`*.png`). To render the scene with the three spheres, while inside the `build` directory, type:

    ./raytrace ../scenes/spheres/spheres.sce output.png

If you have finished all exercise tasks, use

    ./raytrace 0

to render all scenes at once.
    
To set the command line parameters in MSVC or Xcode, please refer to the documentation of these programs (or use the command line...).



Build and Run under Microsoft Windows (Visual Studio)
------------------------------------------------

* Download and install [Visual Studio Community](https://www.visualstudio.com/vs/community/)
* Make sure that you check "Desktop development with C++" during installation
* Download [cmake](https://cmake.org/download/) via the platform windows .zip version and extract it somewhere
* Create an empty build folder inside the project's top-level directory
* Start cmake-gui.exe (located in cmake's bin folder)
* Specify the top-level directory as source directory (button Browse source...)
* Specify the previously created build folder as build directory (button Browse build...)
* Select Configure using your Visual Studio Version as option.
* When configuration is finished, select Generate.
* Start Visual Studio Community
* Open the project via File -> open -> project -> .sln in build folder
* In the project explorer window on the right, right-click the project (raytracer) and set it as startup-project
* Switch to release mode
* The executable will need a scene file (.sce) and the output .png file as command line arguments (or 0 to raytrace all scenes). You can specify those arguments via project -> properties -> debugging -> command arguments
* Hit CTRL + F5 to build and run (or CTRL + SHIFT + B to build)


Dokumentation
-------------

A pre-build html dokumentation can be found in `doc/index.html` and can be opened via any web browser.


Code Overview
-------------

The best way to become familiar with the code is to look at the main function in the file `raytrace.cpp`. It consists of three parts: loading a scene, ray tracing it and write the resulting image. The function `compute_image()` is the starting point for the ray tracer.


Recommended order for the exercises
-----------------------------------

We recommend that you work on the tasks in the following order. All tasks can be found in the ToDo list as well.

Note that, to give you something to compare to, we provide output images created with a completed version of the exercise. They can be found inside the respective scene's directory, e.g. `scenes/spheres/`. Refer to the lecture's slides to get an idea of what the intermediate steps look like.

* **Phong Shading and Reflections (3 points):**
Begin by implementing the Phong shading model. Your starting point will be the `lighting()` function in `raytrace.cpp`. Once you have finished the Phong Shading and get realistic illumination, continue with reflections, which have to be added to `trace()` in `raytrace.cpp`. During coding, ensure you are on the right track by ray tracing the spheres (`scenes/spheres/spheres.sce`).

* **Intersection computation with a plane (1 points):**
To complete proper rendering of the spheres, implement ray-plane intersections (`Plane::intersect()` in `Plane.cpp`).

* **Triangle Meshes (4 points):**
Ray tracing spheres is great (try the scenes with molecules!), but because we want to be able to render more complex scenes, we cannot stop there. The next step is the ray-triangle intersection (`Mesh::intersect_triangle()` in `Mesh.cpp`). Start testing with the cube (`scenes\cube\cube.sce`), which uses flat shading. Then try the office (`scenes\office\office.sce`) or the toon faces (`scenes\toon_faces\toon_faces.sce`). For those scenes, computation of vertex normals needs to be implemented (`Mesh::compute_normals` in `Mesh.cpp`). Implement weighted vertex normals to get a nice result in the rings scene (`scenes\rings\rings.sce`).

* **Acceleration (1 point):**
Are you annoyed because raytracing is damn slow? Let's make things faster! First implement the bounding box test for triangle meshes (`Mesh::intersect_bounding_box()` in `Mesh.cpp`). With this feature you should see a significant performance boost rendering the toon faces (`scenes\toon_faces\toon_faces.sce`). Also try using OpenMP to enable parallelization. For this, simply put
	(`#pragma omp parallel for`)
above the outer loop in during ray generation (`compute_image()` in `raytrace.cpp`).

* **Textures (1 point)**
One color per object is boring. Complete the texture support in `Mesh::intersect_triangle()` and render our cool new Pokemon scene (`scenes\pokemon\pokemon.sce`).

* **Render all scenes:** 
If you finished all the tasks above, you can render all scenes (command line argument 0).
Compare your results to those in the scenes folder. You should get the same pictures, if you did no mistakes.

* **Bonus Task (1 bonus point):**
Do you still want to explore more features? Then why not try super-sampling in order to reduce the aliasing problems? Simply adjust the function `compute_image()`, such that you shoot several rays for each pixel and average their resulting colors. To avoid loosing too much performance, you should implement adaptive supersampling (details in `compute_image()`). There are also 16-ray-supersampled versions of the images in each scene folder for comparison (suffix _SS16). 


