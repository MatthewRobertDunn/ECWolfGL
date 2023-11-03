#version 330 core
out vec4 FragColor;
in vec4 VertexColor;
in vec3 TextureCoords;
uniform sampler2DArray textureArray;



void main()
{
    FragColor = texture(textureArray,TextureCoords) * VertexColor;
    if(FragColor.a < 0.9f){
        discard;
    }
} 