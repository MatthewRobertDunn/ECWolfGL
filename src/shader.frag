#version 330 core
out vec4 FragColor;
in vec4 vertexColor;
in vec3 vertexTexture;


float near = 0.001; 
float far  = 15.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}


void main()
{
    FragColor = vertexColor;
    //FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    //float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    //FragColor = vec4(vec3(depth), 1.0);
} 