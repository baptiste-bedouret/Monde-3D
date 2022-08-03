#version 330 core

in vec3 var_color;
out vec4 out_color;

in vec3 n;
in vec3 v;
in vec3 l;


vec3 blinn(vec3 n, vec3 v, vec3 l, vec3 dCol, vec3 sCol, float s){
  vec3 reflected = -l+2*(dot(l,n));
  float alpha = dot(v,reflected);
  vec3 color= dCol;
  if(alpha>0.0){
    color+= sCol * pow(alpha,s);
  }
  return color;
}

/*vec3 blinn(vec3 n, vec3 v, vec3 l, vec3 dCol, vec3 sCol, float s){
  vec3 halfDir = normalize(l + v);
  float specAngle = max(dot(halfDir, n), 0.0);
  float specular = pow(specAngle, s);
  float ld = max(dot(n,l),0);
  vec3 ambientColor = vec3(0.1, 0.0, 0.0);
  vec3 color= ambientColor+ ld*dCol +sCol*specular;
  return color;
}*/


void main(void) {
  vec3 dCol = vec3(0.5, 0.0, 0.0);
  vec3 sCol = vec3(1.0, 1.0, 1.0);
  float s = 16.0;

  vec3 L = normalize(l);
  vec3 N= normalize(n);
  vec3 blinn=blinn(N,v,L,dCol,sCol,s);
  out_color = vec4(blinn,1.0);
}

/*
void main(void){
  out_color=vec4(var_color,1.0);
}
*/
