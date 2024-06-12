#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color; 
layout (location = 3) in vec3 texture; 
  
//global input, model view projection matrix
uniform mat4 mvp; 
uniform mat4 model;

//These are outputs from the vertex shader and inputs to the fragment shader
out vec4 VertexColor; 
out vec3 TextureCoords; 
out vec3 FragPos;
out vec3 Normal;

void main()
{
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = normal;
    gl_Position =  mvp * vec4(position, 1.0);  //perspective transform on position
    VertexColor = color;
    TextureCoords = vec3(texture.x,1.0 - texture.y,texture.z); //upside down, why?

}