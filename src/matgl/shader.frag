#version 430 core
layout(early_fragment_tests) in;
out vec4 FragColor;
in vec4 VertexColor;
in vec3 TextureCoords;
in vec3 FragPos;
in vec3 Normal;
uniform sampler2DArray textureArray;
uniform vec3 cameraPosition;
uniform vec4 ambientLight;

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

uniform SpotLight spotLight;
uniform SpotLight spotLights[50];
uniform int spotLightsCount;




vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPosition - FragPos);
    vec4 result = mix(texture(textureArray,TextureCoords) * VertexColor, VertexColor, float(TextureCoords.z < 0.0)) * ambientLight;
    result += vec4(CalcSpotLight(spotLight, norm, FragPos, viewDir),0.0);
    
    for(int i=0;i<spotLightsCount;i++){
        result += vec4(CalcSpotLight(spotLights[i], norm, FragPos, viewDir),0.0);
    }
    
    FragColor = result;

    //FragColor = vec4(1.0,0.0,0.0,1.0) * VertexColor;
    //FragColor = VertexColor;
    //FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    //float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    //FragColor = vec4(vec3(depth), 1.0);
    foo;
} 



// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.5f); //shinyness
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    attenuation = clamp(attenuation, 0.0, 1.0);
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
        
    vec3 tex = vec3(mix(texture(textureArray,TextureCoords), VertexColor, float(TextureCoords.z < 0.0)));
    //vec3 tex = vec3(texture(textureArray,TextureCoords) * VertexColor);

    vec3 ambient = light.ambient * tex;
    ambient *= attenuation * intensity;
    return ambient;
}