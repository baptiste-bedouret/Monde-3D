# Monde-3D

The course is an introduction to 3D rendering / image synthesis.
The aim of this course is how to produce an image (=pixel array) from a digital representation
of a 3D scene. 
The main concepts studied here is:
- Ray tracing
- Rasterization: graphics pipeline, z-buffer, OpenGL, shaders
- Space transformations
- Textures, normal maps, reflections & transparency
- Intro to geometric modeling
- Polygonal mesh, subdivision surfaces
- Intro to animation
- etc.

All the different ideas/notions are in folders ex: td1,td2...
I used OpenGl and the code is in c++. 

# How to build 

go inside folder mds3d_tdX

mkdir build

cd build

cmake ..

make -j6

./mds3dXXX

the executable should be working
