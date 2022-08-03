#version 410 core

in vec3 vtx_position;
in vec3 vtx_normal;
in vec3 vtx_color;
out vec3 var_color;

uniform mat4 obj_mat;
uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat3 normal_mat;

out vec3 v;
out vec3 n;
out vec3 l;
uniform vec4 lightPos;

/*
uniform float zoom;
uniform vec2 offset;
*/

void main()
{
  vec4 pos = view_mat * obj_mat * vec4(vtx_position,1.);
  //direction de vue
  v = pos.xyz;
  n = normal_mat * vtx_normal;
  l =  lightPos.xyz - v;
  //vec4 light= lightPos * view_mat;
  //l=light.xyz;
  gl_Position= proj_mat * pos;
  var_color = vtx_color;
}

/*
void main()
{
  gl_Position = vec4((zoom*vtx_position.xy) +offset,-vtx_position.z,1.);
  //gl_Position = vec4(zoom*vtx_position.xy,(-vtx_position.z), 1.);
  var_color=vtx_color;
}
*/