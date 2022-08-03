#version 330 core

in vec3 v_color;
in vec3 v_normal;
in vec3 v_view;
in vec2 texcoord;
in vec3 v_tangent;
in vec3 v_bitangent;

uniform vec3 lightDir;

uniform sampler2D tex2D1;

/*uniform sampler2D tex2D2;
uniform sampler2D tex2D3;
uniform sampler2D texf;*/

uniform sampler2D texnormal;
uniform sampler2D texdiffuse;
uniform sampler2D texearthnormal;
uniform sampler2D texogre;
uniform sampler2D texogrenormal;
uniform sampler2D texdiffuseo;

uniform mat3 normal_mat;

out vec4 out_color;

vec3 blinn(vec3 n, vec3 v, vec3 l, vec3 dCol, vec3 sCol, float s)
{
  vec3 res = vec3(0,0,0);
  float dc = max(0,dot(n,l));
  if(dc>0) {
    res = dCol * dc;
    float sc = max(0,dot(n,normalize(v+l)));
    if(sc>0)
      res += sCol * pow(sc,s) * dc;
  }
  return res;
}

void main(void) {
  float ambient = 0.0;
  float shininess = 50;
  vec3 spec_color = vec3(1,1,1);

  //Affichage sans texture
  //out_color = vec4(ambient * v_color + blinn(normalize(v_normal),normalize(v_view), lightDir, v_color, spec_color, shininess),1.0);

  //Affichage earth
  //out_color=vec4(texcoord,0,1);

  //Affichage earth + atmosphère
  float sol= max(0,dot(normalize(v_normal),lightDir));
  //out_color=sol*(texture(tex2D,texcoord)+texture(tex2Dbis,texcoord));

  //Affichage nuit + atmosphere + earth
  //out_color=sol*mix(texture(tex2D1,texcoord),texture(tex2D2,texcoord),texture(tex2D2,texcoord));
  //out_color=sol*texture(tex2D1,texcoord)+mix(texture(tex2D3,texcoord),sol*texture(tex2D1,texcoord),1);

  //Filtrage
  //out_color=texture(texf,texcoord);

  /***************TD7*********************/
  /*Partie 1:*/

  vec4 t_normal=vec4(2*texture(texnormal,texcoord).rgb-1,0);
  vec3 diffuse= texture(texdiffuse,texcoord).rgb;
  out_color=vec4(ambient * v_color + blinn(normal_mat*normalize(t_normal.rgb),normalize(v_view), lightDir, diffuse, spec_color, shininess),1.0);
  
  /*Partie 2:*/
  //out_color=vec4(ambient * v_color + blinn(normal_mat*normalize(normal.rgb),normalize(v_view), lightDir, diffuse, spec_color, shininess),1.0);
  //out_color=texture(tex2D1,texcoord);

  /*Partie 3:*/
  //vec4 t_normal=vec4(2*texture(texearthnormal,texcoord).rgb-1,0);
  /*vec3 diffuse= texture(texdiffuse,texcoord).rgb;
  vec3 earth = texture(tex2D1, texcoord).rgb;
  vec3 ogre=texture(texogre, texcoord).rgb;

  vec3 tangent=normalize(v_tangent);
  vec3 bitangent=normalize(v_bitangent);
  vec3 normal=normalize(v_normal);

  mat3 tbnvMatrix=transpose(mat3(tangent,bitangent,normal));
  vec3 lt=tbnvMatrix*lightDir;
  vec3 vt=tbnvMatrix*v_view;*/

  /*carte de normales*/
  //out_color=vec4(ambient * v_color + blinn(normal_mat*normalize(t_normal.rgb),normalize(vt), lt, t_normal.rgb, spec_color, shininess),1.0);

  /*Perturbation éclairage diffus*/
  //out_color=vec4(ambient * v_color + blinn(normal_mat*normalize(t_normal.rgb),normalize(vt), lt, v_color, spec_color, shininess),1.0);

  /*résultat final avec la texture de couleur*/
  //out_color=vec4(ambient * v_color + blinn(normalize(t_normal.rgb),normalize(vt), lt, earth, spec_color, shininess),1.0);

  /*test sur l'ogre*/
  //vec4 t_normal=vec4(2*texture(texogrenormal,texcoord).rgb-1,0);
  //vec3 diffuseo= texture(texdiffuseo,texcoord).rgb;
  //out_color=vec4(ambient * v_color + blinn(normal_mat*normalize(normal),normalize(vt), lt,t_normal.rgb, spec_color, shininess),1.0);

}
