#version 330 core

uniform mat4 obj_mat;
uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat3 normal_mat;

in vec3 vtx_position;
in vec3 vtx_normal;
in vec2 vtx_texcoord;

out vec3 v_normal;
out vec2 v_uv;

uniform vec3 A,T1,T2;
float pi = 3.14159265358;
uniform vec3 tangent;
uniform vec3 binormal;
uniform vec3 normal;
out vec3 T;

vec3 S(float u,float v){
  return A+u*T1+v*T2;
}

vec3 cylinder(vec2 uv,float r){
  return vec3(
    uv.x,
    r*cos(2*pi*uv.y),
    r*sin(2*pi*uv.y));
}

vec3 interpolation(vec2 uv, vec3 A,vec3 B){
  return A+uv.x*(B-A);
}

vec3 cylinder2(vec2 uv,float r,vec3 A,vec3 B){
  return interpolation(uv,A,B)
  + vec3(
    uv.x,
    0.1*cos(r*2*pi*uv.y),
    0.1*sin(r*2*pi*uv.y));
}

vec3 cylinder3(vec2 uv, float r,vec3 A,vec3 B){
  /*segment AB*/
  vec3 AB=B-A;
  /*tangent*/
  vec3 tangent=AB;
  /*normal*/
  vec3 dAB=vec3(AB.xy,AB.z+1);
  vec3 normal=normalize(cross(A,dAB));
  /*binormal*/
  vec3 binormal= normalize(cross(normal, tangent));
  /*changement de repère du repère local au repère monde*/
  /*float cx=tangent.x*uv.x+normal.x*uv.y+binormal.x;
  float cy= tangent.y*uv.x+normal.y*uv.y+binormal.y;
  float cz= tangent.z*uv.x+normal.z*uv.y+binormal.z;
  vec3 changement=vec3(cx,cy,cz);*/
  /*vec4 changement=vec4(t.x*uv.x +n.x*uv.y+ b.x*uv.z+o.x,
  t.y*uv.x +n.y*uv.y+ b.y*uv.z+o.y,
  t.z*uv.x +n.z*uv.y+ b.z*uv.z+o.z,1);*/
  mat3 changement=mat3(tangent,normal,binormal);
  vec3 translation=A+uv.x*(B-A);
  return translation+changement*vec3(
    uv.x,
    r*cos(2*pi*uv.y),
    r*sin(2*pi*uv.y));
}

vec3 bezier(float u, vec3 B[4]){
  float s = 1 - u;
  vec3 AB = B[0]*s + B[1]*u;
  vec3 BC = B[1]*s + B[2]*u;
  vec3 CD = B[2]*s + B[3]*u;
  vec3 ABC = AB*s + BC*u;
  vec3 BCD = BC*s + CD*u;
  return ABC*s + BCD*u;
}

vec3 cylBezierYZ(float u, float v, vec3 B[4], float r){
  return
  bezier(u,B)+vec3(
    r*cos(2*pi*v),
    r*sin(2*pi*v),0);
}

vec3 bezierEtendu(float u, vec3 B[4], out vec3 T){
  float s = 1 - u;
  vec3 AB = B[0]*s + B[1]*u;
  vec3 BC = B[1]*s + B[2]*u;
  vec3 CD = B[2]*s + B[3]*u;
  vec3 ABC = AB*s + BC*u;
  vec3 BCD = BC*s + CD*u;
  vec3 ABCD=ABC*s + BCD*u;
  //vecteur tangent:
  T=ABC-BCD;
  return T ;
}

vec3 cylBezierYZEtendu(float u, float v, vec3 B[4], float r){
  vec3 binormal=vec3(0,1,0);
  vec3 tangent=bezierEtendu(u,B,T);
  return
  binormal*tangent*vec3(
    r*cos(2*pi*v),
    r*sin(2*pi*v),0);
}


void main()
{
  float u = vtx_texcoord.x;
  float r=0.1;
  float v= vtx_texcoord.y;
  vec3 A=vec3(0,0,0);
  vec3 B=vec3(2,1,0);
  /*vec3 B[4];
  B[0]=vec3(-1,-0.5,-1);
  B[1]=vec3(-1, 1,-0.3);
  B[2]=vec3(1, -1, 0.3);
  B[3]=vec3 (1,0.5,1);*/
  v_uv  = vtx_texcoord;
  v_normal = normalize(normal_mat * vtx_normal);
  vec4 p = view_mat * (obj_mat * vec4(cylinder(v_uv,r), 1.));
  gl_Position = proj_mat * p;
}
