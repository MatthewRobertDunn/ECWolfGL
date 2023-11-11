#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color; 
layout (location = 3) in vec3 texture; 
  
//global input, model view projection matrix
uniform mat4 mvp; 

//These are outputs from the vertex shader and inputs to the fragment shader
out vec4 VertexColor; 
out vec3 TextureCoords; 


void main()
{
    gl_Position =  mvp * vec4(position, 1.0);  //perspective transform on position
    
    float diff = abs(dot(normal, vec3(1.0,0.0,0.0)));
    diff = max(diff,0.5);
    
    VertexColor = color * vec4(diff,diff,diff,1.0);
    TextureCoords = texture * vec3(1.0,-1.0,1.0); //upside down, why?

}