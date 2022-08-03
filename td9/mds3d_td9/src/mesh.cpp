#include "mesh.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include "shader.h"
#include <Eigen/Geometry>
#include <limits>

using namespace std;
using namespace Eigen;
using namespace surface_mesh;

void Mesh::subdivide()
{
  // TODO !
  auto positions = _halfedge.get_vertex_property<Point>("v:point");
  auto vertex_mapping = _halfedge.add_vertex_property<Surface_mesh::Vertex>("v:mapping");
  auto edge_mapping = _halfedge.add_edge_property<Surface_mesh::Vertex>("e:mapping");
  Surface_mesh new_mesh;
  for (auto v : _halfedge.vertices())
  {
    Surface_mesh::Halfedge h0 = _halfedge.halfedge(v);
    Surface_mesh::Halfedge h = h0;
    Surface_mesh::Vertex vi;
    Vec3 cog = Vec3(0, 0, 0);
    float beta;
    // degré de valence: le nombre d'arete incidente du sommet v
    // int deg = new_mesh.valence(v);
    int deg = 0;

    if (_halfedge.is_boundary(v))
    {
      // On parcours les voisins du sommet vi (juste les voisins au bord)
      do
      {
        if (_halfedge.is_boundary(_halfedge.edge(h)))
        {
          cog += _halfedge.position(_halfedge.to_vertex(h));
        }
        h = _halfedge.next_halfedge(_halfedge.opposite_halfedge(h));
      } while (h != h0);
      vi = new_mesh.add_vertex((3. / 4.) * _halfedge.position(v) + (1. / 8.) * cog);
    }
    // Parcours les voisins du sommet v
    else
    {
      do
      {
        cog += _halfedge.position(_halfedge.to_vertex(h));
        h = _halfedge.next_halfedge(_halfedge.opposite_halfedge(h));
        deg++;
      } while (h != h0);

      if (deg == 3)
      {
        beta = 3. / 16.;
        vi = new_mesh.add_vertex((1 - beta * deg) * _halfedge.position(v) + beta * cog);
      }
      else if (deg > 3)
      {
        beta = (1 / deg) * ((5. / 8.) - pow((3. / 8.) + (1. / 4.) * cos(2 * M_PI / deg), 2));
        vi = new_mesh.add_vertex((1 - beta * deg) * _halfedge.position(v) + beta * cog);
      }
    }
    vertex_mapping[v] = vi;
  }

  // pour chaque arète j'insère un nouveau sommet u
  for (auto e : _halfedge.edges())
  {
    // trouver la position du nouveau sommet:
    Surface_mesh::Vertex u;
    Surface_mesh::Halfedge h0 = _halfedge.halfedge(e, 0);
    Surface_mesh::Vertex v0 = _halfedge.to_vertex(h0);
    Surface_mesh::Vertex v2 = _halfedge.from_vertex(h0);
    Surface_mesh::Halfedge h1 = _halfedge.next_halfedge(h0);
    Surface_mesh::Vertex v1 = _halfedge.to_vertex(h1);
    Surface_mesh::Vertex v3 = _halfedge.to_vertex(_halfedge.next_halfedge(_halfedge.opposite_halfedge(h0)));
    if (_halfedge.is_boundary(h0))
    {
      u = new_mesh.add_vertex((1. / 2.) * (positions[v0] + positions[v2]));
    }
    else
    {
      u = new_mesh.add_vertex((3. / 8.) * positions[v0] + (3. / 8.) * positions[v2] + (1. / 8.) * positions[v1] + (1. / 8.) * positions[v3]);
    }
    edge_mapping[e] = u;
  }

  // comment recreer les faces?
  // On fait une boucle pour chacune des faces
  // voir diapo s'inspirer structure de code et rajouter une boucle for

  for (auto f : _halfedge.faces())
  {
    Surface_mesh::Edge es[3];
    Surface_mesh::Vertex vs[3];
    int i = 0;
    Surface_mesh::Halfedge h0 = _halfedge.halfedge(f);
    Surface_mesh::Halfedge h = h0;
    do
    {
      assert(i < 3);
      vs[i] = _halfedge.to_vertex(h);
      es[i] = _halfedge.edge(h);
      h = _halfedge.next_halfedge(h);
      i++;
    } while (h != h0);
    new_mesh.add_triangle(edge_mapping[es[0]], vertex_mapping[vs[0]], edge_mapping[es[1]]);
    new_mesh.add_triangle(edge_mapping[es[1]], vertex_mapping[vs[1]], edge_mapping[es[2]]);
    new_mesh.add_triangle(vertex_mapping[vs[2]], edge_mapping[es[0]], edge_mapping[es[2]]);
    new_mesh.add_triangle(edge_mapping[es[2]], edge_mapping[es[0]], edge_mapping[es[1]]);
  }
  new_mesh.update_face_normals();
  new_mesh.update_vertex_normals();
  _halfedge = new_mesh;
  updateHalfedgeToMesh();
  updateVBO();
}

Mesh::~Mesh()
{
  if (_isInitialized)
  {
    glDeleteBuffers(1, &_vertexBufferId);
    glDeleteBuffers(1, &_indexBufferId);
    glDeleteVertexArrays(1, &_vertexArrayId);
  }
}

bool Mesh::load(const std::string &filename)
{
  std::cout << "Loading: " << filename << std::endl;

  if (!_halfedge.read(filename))
    return false;

  _halfedge.update_face_normals();
  _halfedge.update_vertex_normals();

  updateHalfedgeToMesh();

  return true;
}

void Mesh::updateHalfedgeToMesh()
{
  // vertex properties
  Surface_mesh::Vertex_property<Point> vertices = _halfedge.get_vertex_property<Point>("v:point");
  Surface_mesh::Vertex_property<Point> vnormals = _halfedge.get_vertex_property<Point>("v:normal");
  Surface_mesh::Vertex_property<Texture_coordinate> texcoords = _halfedge.get_vertex_property<Texture_coordinate>("v:texcoord");
  Surface_mesh::Vertex_property<Color> colors = _halfedge.get_vertex_property<Color>("v:color");

  // vertex iterator
  Surface_mesh::Vertex_iterator vit;

  Vector3f pos;
  Vector3f normal;
  Vector2f tex;
  Vector4f color;
  _vertices.clear();
  for (vit = _halfedge.vertices_begin(); vit != _halfedge.vertices_end(); ++vit)
  {
    pos = vertices[*vit];
    normal = vnormals[*vit];
    if (texcoords)
      tex = texcoords[*vit];
    if (colors)
      color << colors[*vit], 1.0f;
    else
      color = Vector4f(0.6f, 0.6f, 0.6f, 1.0f);

    _vertices.push_back(Vertex(pos, normal, color, tex));
  }

  // face iterator
  Surface_mesh::Face_iterator fit, fend = _halfedge.faces_end();
  // vertex circulator
  Surface_mesh::Vertex_around_face_circulator fvit, fvend;
  Surface_mesh::Vertex v0, v1, v2;
  _faces.clear();
  for (fit = _halfedge.faces_begin(); fit != fend; ++fit)
  {
    fvit = fvend = _halfedge.vertices(*fit);
    v0 = *fvit;
    ++fvit;
    v2 = *fvit;

    do
    {
      v1 = v2;
      ++fvit;
      v2 = *fvit;
      _faces.push_back(Vector3i(v0.idx(), v1.idx(), v2.idx()));
    } while (++fvit != fvend);
  }

  // updateNormals();
}

void Mesh::init()
{
  glGenVertexArrays(1, &_vertexArrayId);
  glGenBuffers(1, &_vertexBufferId);
  glGenBuffers(1, &_indexBufferId);

  updateVBO();

  _isInitialized = true;
}

void Mesh::updateNormals()
{
  // pass 1: set the normal to 0
  for (std::vector<Vertex>::iterator v_iter = _vertices.begin(); v_iter != _vertices.end(); ++v_iter)
    v_iter->normal.setZero();

  // pass 2: compute face normals and accumulate
  for (std::size_t j = 0; j < _faces.size(); ++j)
  {
    Vector3f v0 = _vertices[_faces[j][0]].position;
    Vector3f v1 = _vertices[_faces[j][1]].position;
    Vector3f v2 = _vertices[_faces[j][2]].position;

    Vector3f n = (v1 - v0).cross(v2 - v0).normalized();

    _vertices[_faces[j][0]].normal += n;
    _vertices[_faces[j][1]].normal += n;
    _vertices[_faces[j][2]].normal += n;
  }

  // pass 3: normalize
  for (std::vector<Vertex>::iterator v_iter = _vertices.begin(); v_iter != _vertices.end(); ++v_iter)
    v_iter->normal.normalize();
}

void Mesh::updateVBO()
{
  glBindVertexArray(_vertexArrayId);

  // activate the VBO:
  glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
  // copy the data from host's RAM to GPU's video memory:
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), _vertices[0].position.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vector3i) * _faces.size(), _faces[0].data(), GL_STATIC_DRAW);
}

void Mesh::draw(const Shader &shd)
{
  if (!_isInitialized)
    init();

  // Activate the VBO of the current mesh:
  glBindVertexArray(_vertexArrayId);
  glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);

  // Specify vertex data

  // 1 - get id of the attribute "vtx_position" as declared as "in vec3 vtx_position" in the vertex shader
  int vertex_loc = shd.getAttribLocation("vtx_position");
  if (vertex_loc >= 0)
  {
    // 2 - tells OpenGL where to find the x, y, and z coefficients:
    glVertexAttribPointer(vertex_loc,     // id of the attribute
                          3,              // number of coefficients (here 3 for x, y, z)
                          GL_FLOAT,       // type of the coefficients (here float)
                          GL_FALSE,       // for fixed-point number types only
                          sizeof(Vertex), // number of bytes between the x coefficient of two vertices
                                          // (e.g. number of bytes between x_0 and x_1)
                          0);             // number of bytes to get x_0
    // 3 - activate this stream of vertex attribute
    glEnableVertexAttribArray(vertex_loc);
  }

  int normal_loc = shd.getAttribLocation("vtx_normal");
  if (normal_loc >= 0)
  {
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)sizeof(Vector3f));
    glEnableVertexAttribArray(normal_loc);
  }

  int color_loc = shd.getAttribLocation("vtx_color");
  if (color_loc >= 0)
  {
    glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(2 * sizeof(Vector3f)));
    glEnableVertexAttribArray(color_loc);
  }

  int texcoord_loc = shd.getAttribLocation("vtx_texcoord");
  if (texcoord_loc >= 0)
  {
    glVertexAttribPointer(texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(2 * sizeof(Vector3f) + sizeof(Vector4f)));
    glEnableVertexAttribArray(texcoord_loc);
  }

  // send the geometry
  glDrawElements(GL_TRIANGLES, (gl::GLsizei)(3 * _faces.size()), GL_UNSIGNED_INT, 0);

  // at this point the mesh has been drawn and raserized into the framebuffer!
  if (vertex_loc >= 0)
    glDisableVertexAttribArray(vertex_loc);
  if (normal_loc >= 0)
    glDisableVertexAttribArray(normal_loc);
  if (color_loc >= 0)
    glDisableVertexAttribArray(color_loc);
  if (texcoord_loc >= 0)
    glDisableVertexAttribArray(texcoord_loc);

  checkError();
}
