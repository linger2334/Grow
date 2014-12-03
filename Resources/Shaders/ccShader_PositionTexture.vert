
attribute vec4 a_position;
attribute vec2 a_texCoord;

#ifdef GL_ES
varying mediump vec2 v_texCoord;
//varying mediump vec4 gl_Color;
#else
varying vec2 v_texCoord;
//varying vec4 gl_Color;
#endif


void main()
{
    gl_Position = CC_MVPMatrix * a_position;
    //gl_Color    = gl_FrontColor;
    v_texCoord = a_texCoord;
}
