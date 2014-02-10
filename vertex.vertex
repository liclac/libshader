#version 110

attribute vec2 position;

varying vec2 texcoord;

/*void main()
{
    // Transforming The Vertex
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    
    // Passing The Texture Coordinate Of Texture Unit 0 To The Fragment Shader
    texture_coordinate = vec2(gl_MultiTexCoord0);
}*/

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    texcoord = position * vec2(0.5) + vec2(0.5);
}
