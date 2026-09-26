<h1>25809221_ProgrammingAssignment1</h1>

<h2>Description</h2>
 A C++ ray tracer with a command-line scene editor.

 Users can create, modify and delete objects within a scene and render the resulting scene to a .ppm file.
 The .ppm output can then be converted to another image format using image-conversion software of the user's choice.

 <h2>How To Build/Run</h2>

The program requires **C++20**.

Build using:
**g++ -std=c++20 -Iinclude src/main.cpp src/cli.cpp src/scene.cpp -o 25809221_ProgrammingAssignment1**

The program outputs rendered images in .ppm format. An image converter is required to view the rendered images in 
most standard image viewers.

To begin using the program, enter the command 'help'.
Note that when running the 'show' command, variable names are as shown and should be used when the user tries to
modify these properties.

<h2>Additional Information</h2>

<h3>Viewing Rendered Images</h3>

The rendered .ppm files can be converted to a standard image format using image-conversion software.

I have been using ImageMagick for this purpose:

    https://imagemagick.org/ 

For example:

    Magick <scene_name>.ppm <image_name>.png

<h3>3D Worldspace</h3>

Positions can be interpreted as (x, y, z) in a 3D space.

    Positive x goes right. Negative x goes left.

    Positive y goes up. Negative y goes down.

    Positive z goes towards the camera. Negative z goes away from the camera.

<h3>Camera</h3>

The camera is positioned at: 

    (0, 0, 0)

and faces forwards in the **negative z direction**.

If an object does not appear in the rendered image, check that it's position is within the camera's view.

<h3>Scene and Object Information</h3>

Scene properties can be specified (while editing) using:

    > scene_name <new scene_name>
    > aspect_ratio <double>
    > image_width <int>
    > anti_aliasing <int>
    > max_depth <int>
    > skybox_colour_i <double> <double> <double>
    > skybox_colour_j <double> <double> <double>

Scenes have default values:

    > aspect_ratio 1
    > image_width 100
    > anti_aliasing 10
    > max_depth 10
    > skybox_colour_i 0 0 0
    > skybox_colour_j 1 1 1

Sphere properties can be specified using:

    > radius <double>
    > center <double> <double> <double>

Spheres have default values:

    > radius 0.5
    > center 0 0 -1

Note that object_name cannot be changed after an object has been created, and may not contain spaces.

<h3>Saving Scenes</h3>

Scene changes are saved automatically. However, scene files update when the user exits scene editing. Do not attempt to
close the program while editing a scene, as the program may behave unexpectedly.

<h3>Scene File Formatting</h3>

Scene data is stored in .txt files in a specific format. The following is an example of a *newly created scene* with
default values:

    scene_name=scene
    aspect_ratio=1
    image_width=100
    samples_per_pixel=10
    max_depth=10
    skybox_colour_i=0,0,0
    skybox_colour_j=1,1,1

    [Objects]

The following is an example of a *modified scene* with added objects:

    scene_name=scene_mod
    aspect_ratio=1
    image_width=450
    samples_per_pixel=10
    max_depth=10
    skybox_colour_i=0,0,0
    skybox_colour_j=1,1,1

    [Objects]
    sphere,sphere0,-0.5,-0.3,-1,0.2
    sphere,sphere1,0.9,0.9,-2,0.9
    sphere,sphere2,0.1,0.4,-4.5,1.3

<h3>Potential Issues</h3>

If the program crashes when it is run, there may be an error in the formatting of one the scene files.

If this occurs, either delete the affect .txt scene files or manually correct its formatting.

Generally, scene.txt files should not be edited manually, as incorrect formatting may cause the program to
behave unexpectedly or crash.
