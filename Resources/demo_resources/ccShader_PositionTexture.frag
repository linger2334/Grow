
#ifdef GL_ES
precision lowp float;
#endif

varying vec2 v_texCoord;
varying vec4 v_color;

void main()
{
   // vec4 color = texture2D(CC_Texture0, v_texCoord);
    //if (v_color.a < 1.0) {
    //    discard;
    //}
//    if (v_texCoord.x<0.5) {
//      //  gl_FragColor = gl_Color;
//    }
//    else
//    {
    gl_FragColor = texture2D(CC_Texture0, v_texCoord);
   // }
}

