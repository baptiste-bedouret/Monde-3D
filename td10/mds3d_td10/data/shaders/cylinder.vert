#version 330 core

const float M_PI = 3.14159265359;

uniform mat4 obj_mat;
uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat3 normal_mat;
uniform int cyl;
uniform int _length[3];

uniform mat4 skinning1;
uniform mat4 skinning2;
uniform mat4 skinning3;

mat4 segment(vec2 uv){
    int sum = _length[0]+_length[1]+_length[2];
    if(uv.y<= _length[0]/sum)
        return skinning1;
    if(uv.y<=(_length[0]+_length[1])/sum && uv.y>_length[0]/sum)
        return skinning2;
    else
        return skinning3;
}

in vec3 vtx_position;
in vec3 vtx_normal;
in vec2 vtx_texcoord;

out vec3 v_normal;
out vec3 v_view;
out vec2 v_uv;

vec3 cylinder(vec2 uv, float r, float l)
{
    return vec3(r*cos(uv.x*2*M_PI), r*sin(uv.x*2*M_PI), l*uv.y);
}

void main()
{
    v_uv  = vtx_texcoord;
    vec4 p;
    vec4 pi;

    if(cyl==1) {
        vec3 pos = cylinder(v_uv, 0.5, 1);
        vec3 normal = vec3(pos.x,pos.y,0);
        v_normal = normalize(normal_mat * normal);
        if(v_uv.y <= _length[0])
            p = view_mat * (skinning1* obj_mat * vec4(pos, 1.));
        else if(v_uv.y > _length[0] && v_uv.y <= _length[1])
            p = view_mat * (skinning2 * obj_mat * vec4(pos, 1.));
        //else //if(v_uv.y > _length[1]/sum && v_uv.y <= _length[2]/sum)
        else p =  view_mat * (skinning3 *obj_mat * vec4(pos, 1.));
        //else
            //p = view_mat * (obj_mat * vec4(pos, 1.));

    }else{
        v_normal = normalize(normal_mat * vtx_normal);
        p = view_mat * (obj_mat * vec4(vtx_position, 1.));
    }
    v_view = normalize(-p.xyz);
    gl_Position = proj_mat * p;
}
