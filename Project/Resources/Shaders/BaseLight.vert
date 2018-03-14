attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;

varying vec2 v_texCoord;
varying vec4 uv_color;

uniform mat4 u_matProjectionView;
uniform mat4 u_matViewModel;

uniform vec3 u_lightDirection;

void main(void)
{
    vec4 u_color = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 u_ambientColor = vec4(0.0,0.0,0.0,1.0);
    float u_diffuse = 1.0;
    
    mat4 u_transform = u_matProjectionView * u_matViewModel;
    
    gl_Position = u_transform * a_position;
    
    vec4 n = u_matViewModel * vec4(a_normal, 0.0);
    
    vec3 v_normal = normalize(n.xyz);
    v_texCoord = a_texCoord;
  
    float lcoef = max(dot(v_normal, -u_lightDirection.xyz), 0.0);

    uv_color = u_ambientColor;
    uv_color.rgb += lcoef * (u_diffuse);
    uv_color *= u_color;
}

