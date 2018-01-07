uniform mat4 u_matrix;
uniform float u_pointSize;

attribute vec4	a_position;

void main()
{
	gl_PointSize = u_pointSize;
	gl_Position = u_matrix * a_position;
}
