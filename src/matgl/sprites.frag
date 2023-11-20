#version 330 core
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

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPosition - FragPos);
    vec4 result = texture(textureArray,TextureCoords) * VertexColor * ambientLight;
    result += vec4(CalcSpotLight(spotLight, norm, FragPos, viewDir),0.0);
    FragColor = result;

    if(FragColor.a < 0.9f){
        discard;
    }
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
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
        
    vec3 tex = vec3(mix(texture(textureArray,TextureCoords), VertexColor, TextureCoords.z < 0.0));
    
    vec3 ambient = light.ambient * tex;
    ambient *= attenuation * intensity;
    return ambient;
}