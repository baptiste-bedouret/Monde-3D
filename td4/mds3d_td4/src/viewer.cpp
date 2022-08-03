#include "viewer.h"
#include "camera.h"

using namespace Eigen;

Viewer::Viewer()
    : _winWidth(0), _winHeight(0)
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

  // if(!_mesh.load(DATA_DIR"/models/quad.off")) exit(1);
  if (!_mesh.load(DATA_DIR "/models/sphere.obj"))
    exit(1);
  _mesh.initVBA();
  reshape(w, h);
  _cam.setPerspective(M_PI / 3, 1.f, 200000.0f);
  _cam.lookAt(Vector3f(0, 1, 0), Vector3f(0, 0, 0), Vector3f(1, 2, 1));
  //_cam.rotateAroundTarget(M_PI, Vector3f(0, 1, 0));
  _trackball.setCamera(&_cam);
  glClearColor(0.5, 0.5, 0.5, 1);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
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
  glViewport(0, 0, _winWidth, _winHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _shader.activate();

  glUniform1i(_shader.getUniformLocation("wireframe"), 0);

  _mesh.draw(_shader);

  if (_wireframe)
  {
    glUniform1i(_shader.getUniformLocation("wireframe"), 1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDepthFunc(GL_LEQUAL);

    _mesh.draw(_shader);

    glUniform1i(_shader.getUniformLocation("wireframe"), 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthFunc(GL_LESS);
  }

  _shader.deactivate();
}

void Viewer::drawScene2D()
{
  _shader2D.activate();

  // 2D:
  //  mise à l'échelle et translation de l'objet
  Matrix4f M;
  M << _zoom, 0, 0, 0,
      0, _zoom, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1;

  /***********************************************TD4*************************************************/
  /*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Chaise 1:
  Affine3f A0;
  //  d'abord j'applique une translation  à l'objet puis je met à l'échelle:
  A0 = Translation3f(-0.6, -1.0, 0) * Scaling(0.5f, 0.5f, 1.f);
  glUniformMatrix4fv(_shader2D.getUniformLocation("obj_mat"), 1, GL_FALSE, A0.data());

  glViewport(0, 0, _winWidth / 2, _winHeight);
  drawScene2D();

  // Chaise 2:
  Affine3f A1;
  //  d'abord je met a l'échelle mon objet et puis je lui applique la translation:
  A1 = Scaling(0.5f, 0.5f, 1.f) * Translation3f(-0.6, -1.0, 0);
  glUniformMatrix4fv(_shader2D.getUniformLocation("obj_mat"), 1, GL_FALSE, A1.data());

  glViewport(_winWidth / 2, 0, _winWidth / 2, _winHeight);
  drawScene2D();

  // rotation selon l'axe z
  Vector3f center(0, 0.5, 0);
  Affine3f A2 = Translation3f(center) * AngleAxisf(_zoom, Vector3f::UnitZ());
  glUniformMatrix4fv(_shader2D.getUniformLocation("obj_mat"), 1, GL_FALSE, A2.data());

  glViewport(_winWidth / 2, 0, _winWidth / 2, _winHeight / 3);
  drawScene2D();*/
  /**********************************************TD5*************************************************/

  // ajouter vecteur constant lightpos
  glUniform4f(_shader2D.getUniformLocation("lightPos"), 1, 1, 1, 1);

  Affine3f A3 = Translation3f(0, 0, 0) * Scaling(0.5f, 0.5f, 1.f);
  glUniformMatrix4fv(_shader2D.getUniformLocation("obj_mat"), 1, GL_FALSE, A3.data());

  // matrice de perspective/projection
  Matrix4f proj = _cam.projectionMatrix();
  proj.setIdentity();
  glUniformMatrix4fv(_shader2D.getUniformLocation("proj_mat"), 1, GL_FALSE, proj.data());

  // matrice de vue
  Matrix4f view = _cam.viewMatrix();
  view.setIdentity();
  glUniformMatrix4fv(_shader2D.getUniformLocation("view_mat"), 1, GL_FALSE, view.data());

  // Matrice de transformation des normales
  Matrix4f R = view * A3.matrix();
  Matrix3f L = R.topLeftCorner(3, 3);
  Matrix3f normal = (L.inverse()).transpose();
  glUniformMatrix4fv(_shader2D.getUniformLocation("normal_mat"), 1, GL_FALSE, normal.data());

  // Paramètres pour le zoom et le décalage
  glUniform1f(_shader2D.getUniformLocation("zoom"), _zoom);
  glUniform2fv(_shader2D.getUniformLocation("offset"), 1, _offset.data());

  _mesh.draw(_shader2D);
  _shader2D.deactivate();
}

void Viewer::updateAndDrawScene()
{

  /************************************TD4*******************************************/
  // drawScene2D();
  /************************************TD5*******************************************/

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, _winWidth, _winHeight);
  drawScene2D();
}

void Viewer::loadShaders()
{
  // Here we can load as many shaders as we want, currently we have only one:
  _shader2D.loadFromFiles(DATA_DIR "/shaders/simple.vert", DATA_DIR "/shaders/simple.frag");
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
      _offset.y() += 0.05f;
    }
    else if (key == GLFW_KEY_DOWN)
    {
      _offset.y() -= 0.05f;
    }
    else if (key == GLFW_KEY_LEFT)
    {
      _offset.x() -= 0.05f;
    }
    else if (key == GLFW_KEY_RIGHT)
    {
      _offset.x() += 0.05f;
    }
    else if (key == GLFW_KEY_PAGE_UP)
    {
      _zoom *= 1.1f;
    }
    else if (key == GLFW_KEY_PAGE_DOWN)
    {
      _zoom /= 1.1f;
    }
    else if (key = GLFW_KEY_W)
    {
      _wireframe = !_wireframe;
    }
    else if (key = GLFW_KEY_Z)
    {
      _zoom *= M_PI;
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
  _cam.zoom(-0.1f * y);
}

void Viewer::charPressed(int /*key*/)
{
}
