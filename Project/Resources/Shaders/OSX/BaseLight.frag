varying vec2 v_texCoord;
uniform sampler2D s_baseMap;

varying vec4 uv_color;

void main(void)
{
    vec4 color = uv_color * texture2D(s_baseMap, v_texCoord).bgra;
    gl_FragColor = color;
}

