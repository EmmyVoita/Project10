#version 330 core
out vec4 FragColor; // Returns FragColor

in vec2 TexCoords;
in vec3 Normal; // Receives Normal
in vec3 FragPos; // Receives FragPos
  
uniform vec3 lightPos; // Receives lightPos uniform
uniform vec3 viewPos; // Receives viewPos uniform
uniform vec3 lightColor; // Recieves lightColor uniform
uniform vec3 cylinderColor; // Receives cylinderColor uniform

uniform sampler2D bumpMapTexture;
uniform sampler2D diffuseTexture;
 


vec3 bump_map(vec2 TexCoord)
{   

     // Sample the height map texture and multiply it by the height scale factor
    //float height = texture(bumpMapTexture, TexCoord).r * 3.0;

    // Sample the normal map texture and convert it from tangent space to view space
    vec3 T = normalize(vec3(dFdx(TexCoord), 0.0));
    vec3 B = cross(Normal, T);
    mat3 TBN = mat3(T, B, Normal);

    float bumpiness = texture(bumpMapTexture, TexCoord).r;
    //vec3 perturb = vec3(0.0); // Initialize to default value
    vec3 perturb = TBN * vec3((bumpiness * 2.0 - 1.0));
    perturb = normalize(perturb);

    vec3 normal = normalize(Normal + perturb * 0.5);

    return normal;
}



void main()
{

    vec3 norm = bump_map(TexCoords);
    //modify the fragemnt normal using the bump map/normal map
    //vec3 normal = Normal + texture(bumpMapTexture, TexCoords).rgb;

    // ambient
    float ambientStrength = 0.2;  // Set ambient strength
    vec3 ambient = ambientStrength * lightColor;  // Sets ambient - multiplies strength decimal by light color
  	
    // diffuse 
    //vec3 norm = normalize(normal);  // Normalizes normal
    vec3 lightDir = normalize(lightPos - FragPos);  // Sets light direction based on light - frag position
    float diff = max(dot(norm, lightDir), 0.0);  // Diff value based on max method and dot product
    vec3 diffuse = diff * lightColor;  // Sets diffuse
    
    // specular
    float specularStrength = 0.1f;  // Sets specular strength
    vec3 viewDir = normalize(viewPos - FragPos);  // Sets view direction
    vec3 reflectDir = reflect(-lightDir, norm);  // Sets reflect direction
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);  // Sets specular based on power, max, and dot product
    vec3 specular = specularStrength * spec * lightColor;  // Sets specular
        
    vec3 result = (ambient + diffuse) * texture(diffuseTexture, TexCoords).rgb + specular;  // Adds ambient, diffuse, and specular and multiplies by wall color
    FragColor = vec4(result, 1.0f);  // Sets vec4 based on result
} 
