#version 330 core
out vec4 FragColor;
in vec4 VertexColor;
in vec3 TextureCoords;
in vec3 FragPos;
in vec3 Normal;
uniform sampler2DArray textureArray;
uniform vec4 ambientLight;



void main()
{
    vec4 result = texture(textureArray,TextureCoords) * VertexColor;
    FragColor = result;

    if(FragColor.a < 0.9f){
        discard;
    }
}
