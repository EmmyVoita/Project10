#version 330 core
out vec4 FragColor; // Returns FragColor

in vec3 Normal; // Receives normal
in vec3 FragPos; // Receives FragPos

uniform vec3 lightPos; // Receives lightPos uniform
uniform vec3 viewPos; // Receives viewPos uniform
uniform vec3 lightColor; // Receives lightColor uniform
uniform samplerCube skybox; // Receives skybox texture

void main() 
{
    vec3 I = normalize(FragPos-viewPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox,R).rgb, 1.0);
}


