#ifndef VIEWER_H
#define VIEWER_H

#include "opengl.h"
#include "shader.h"
#include "camera.h"
#include "trackball.h"
#include "mesh.h"

#include <iostream>

class Viewer
{
public:
  //! Constructor
  Viewer();
  virtual ~Viewer();

  // gl stuff
  void init(int w, int h);
  void drawScene();
  void updateAndDrawScene();
  void reshape(int w, int h);
  void loadShaders();

  // events
  void mousePressed(GLFWwindow *window, int button, int action);
  void mouseMoved(int x, int y);
  void mouseScroll(double x, double y);
  void keyPressed(int key, int action, int mods);
  void charPressed(int key);

private:
  int _winWidth, _winHeight;
  GLuint tex2D1, tex2D2, tex2D3, texf, _texID1, texnormal, texdiffuse, texearthnormal, texogre, texogrenormal, texdiffuseo;

  GLuint _samplerId;
  GLenum _minFilter = GL_NEAREST;
  GLenum _magFilter = GL_NEAREST;
  float _zoom = 1;

  Camera _cam;
  Shader _shader;
  Mesh _mesh;

  float _theta;
  bool _rotate;

  // Mouse parameters for the trackball
  enum TrackMode
  {
    TM_NO_TRACK = 0,
    TM_ROTATE_AROUND,
    TM_ZOOM,
    TM_LOCAL_ROTATE,
    TM_FLY_Z,
    TM_FLY_PAN
  };
  TrackMode _trackingMode = TM_NO_TRACK;
  Trackball _trackball;
  Eigen::Vector2i _lastMousePos;
};

#endif
