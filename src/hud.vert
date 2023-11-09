#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color; 
layout (location = 2) in vec3 texture; 
  
//global input, model view projection matrix
uniform mat4 mvp; 

//These are outputs from the vertex shader and inputs to the fragment shader
out vec4 VertexColor; 
out vec3 TextureCoords; 

//pretty much an identity shader for the HUD elements
void main()
{
    gl_Position =  vec4(position,1.0);
    VertexColor = color;
    TextureCoords = texture;
}