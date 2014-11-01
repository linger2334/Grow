
#ifdef GL_ES
precision mediump float;
#endif


varying vec2 v_texCoord;
varying vec4 v_color;

void main()
{
 //  vec4 color = texture2D(CC_Texture0, v_texCoord);

    gl_FragColor = texture2D(CC_Texture0, v_texCoord) ;
 
}


