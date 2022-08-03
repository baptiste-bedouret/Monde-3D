#include "viewer.h"
#include "camera.h"

using namespace Eigen;

Viewer::Viewer()
    : _winWidth(0), _winHeight(0), _zoom(2)
{
}

Viewer::~Viewer()
{
}

////////////////////////////////////////////////////////////////////////////////
// GL stuff

// initialize OpenGL context
void Viewer::init(int w, int h)
{

  loadShaders();
  if (!_mesh.load(DATA_DIR "/models/lemming.off"))
    exit(1);
  _mesh.initVBA();
  glClearColor(0.5, 0.5, 0.5, 0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
  reshape(w, h);
  _trackball.setCamera(&_cam);
}

void Viewer::reshape(int w, int h)
{
  _winWidth = w;
  _winHeight = h;
  _cam.setViewport(w, h);
}

/*!
   callback to draw graphic primitives
 */

void Viewer::drawScene()
{
  // TODO
  // glViewport(0, 0, _winWidth, _winHeight);
  _shader.activate();
  glUniform1f(_shader.getUniformLocation("zoom"), _zoom);
  glUniform2fv(_shader.getUniformLocation("offset"), 1, _offset.data());
  glUniform1i(_shader.getUniformLocation("mode"), 0);
  _mesh.draw(_shader);

  if (_wireframe)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUniform1i(_shader.getUniformLocation("mode"), 1);
    _mesh.draw(_shader);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  _shader.deactivate();
}

void Viewer::updateAndDrawScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  _shader.activate();
  glViewport(0, 0, _winWidth / 2, _winHeight);
  glUniform1i(_shader.getUniformLocation("rot"), 0);
  drawScene();

  _shader.activate();
  glViewport(_winWidth / 2, 0, _winWidth / 2, _winHeight);
  glUniform1i(_shader.getUniformLocation("rot"), 1);
  drawScene();
}

void Viewer::loadShaders()
{
  // Here we can load as many shaders as we want, currently we have only one:
  _shader.loadFromFiles(DATA_DIR "/shaders/simple.vert", DATA_DIR "/shaders/simple.frag");
  checkError();
}

////////////////////////////////////////////////////////////////////////////////
// Events

/*!
   callback to manage keyboard interactions
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::keyPressed(int key, int action, int /*mods*/)
{
  if (key == GLFW_KEY_R && action == GLFW_PRESS)
  {
    loadShaders();
  }

  if (action == GLFW_PRESS || action == GLFW_REPEAT)
  {
    if (key == GLFW_KEY_UP)
    {
      _offset.y() += 0.05;
    }
    else if (key == GLFW_KEY_DOWN)
    {
      _offset.y() -= 0.05;
    }
    else if (key == GLFW_KEY_LEFT)
    {
      _offset.x() -= 0.05;
    }
    else if (key == GLFW_KEY_RIGHT)
    {
      _offset.x() += 0.05;
    }
    else if (key == GLFW_KEY_PAGE_UP)
    {
      _zoom *= 1.1;
    }
    else if (key == GLFW_KEY_PAGE_DOWN)
    {
      _zoom /= 1.1;
    }
    else if (key == GLFW_KEY_W)
    {
      _wireframe = !_wireframe;
    }
  }
}

/*!
   callback to manage mouse : called when user press or release mouse button
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::mousePressed(GLFWwindow * /*window*/, int /*button*/, int action)
{
  if (action == GLFW_PRESS)
  {
    _trackingMode = TM_ROTATE_AROUND;
    _trackball.start();
    _trackball.track(_lastMousePos);
  }
  else if (action == GLFW_RELEASE)
  {
    _trackingMode = TM_NO_TRACK;
  }
}

/*!
   callback to manage mouse : called when user move mouse with button pressed
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::mouseMoved(int x, int y)
{
  if (_trackingMode == TM_ROTATE_AROUND)
  {
    _trackball.track(Vector2i(x, y));
  }

  _lastMousePos = Vector2i(x, y);
}

void Viewer::mouseScroll(double /*x*/, double y)
{
  _cam.zoom(-0.1 * y);
}

void Viewer::charPressed(int /*key*/)
{
}
