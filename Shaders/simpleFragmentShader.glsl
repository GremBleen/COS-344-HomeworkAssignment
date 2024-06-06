#version 330 core

struct dir_light {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct point_light {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct spot_light {
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

in vec3 fragPosition;
in vec3 fragNormal;
in vec4 fragColour;
in vec2 fragTexCoord;

out vec4 FragColour;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColour;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

vec4 cacl_dir_light(dir_light light, vec3 normal, vec3 viewDir)
{
	vec3 lightDirection = normalize(-light.direction);
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	// Combine results
	vec3 ambient = light.ambient * vec3(texture(diffuse0, fragTexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(diffuse0, fragTexCoord));
	vec3 specular = light.specular * spec * vec3(texture(specular0, fragTexCoord));
	return vec4(ambient + diffuse + specular, 1.0);
}

vec4 point_light(float a, float b)
{
	vec3 light_dir_vector = lightPosition - fragPosition;
	float dist = length(light_dir_vector);

	// Constants for inverse square law
	float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// Ambient
	float ambientStrength = 0.2f;
	vec4 ambient = ambientStrength * lightColour;

	// Diffuse
	vec3 normal = normalize(fragNormal);
	vec3 lightDirection = normalize(light_dir_vector);
	float diff = max(dot(normal, lightDirection), 0.0f);
	vec4 diffuse = diff * lightColour;

	// Specular
	float specularStrength = 1.0f;
	vec3 viewDirection = normalize(cameraPosition - fragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 8);
	vec4 specular = spec * specularStrength * lightColour;

	vec4 result = texture(diffuse0, fragTexCoord) * (ambient + diffuse * intensity) + texture(specular0, fragTexCoord).r * (specular * intensity); 
	return result;
}

vec4 directional_light(vec3 light_dir_vector)
{
	// Ambient
	float ambientStrength = 0.2f;
	vec4 ambient = ambientStrength * lightColour;

	// Diffuse
	vec3 normal = normalize(fragNormal);
	vec3 lightDirection = normalize(light_dir_vector);
	float diff = max(dot(normal, lightDirection), 0.0f);
	vec4 diffuse = diff * lightColour;

	// Specular
	float specularStrength = 0.5f;
	vec3 viewDirection = normalize(cameraPosition - fragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 8);
	vec4 specular = spec * specularStrength * lightColour;

	vec4 result = texture(diffuse0, fragTexCoord) * (ambient + diffuse) + texture(specular0, fragTexCoord).r * (specular); 
	return result;
}

vec4 spot_light(float inner, float outer, vec3 spot_light_dir)
{
	// Ambient
	float ambientStrength = 0.2f;
	vec4 ambient = ambientStrength * lightColour;

	// Diffuse
	vec3 normal = normalize(fragNormal);
	vec3 lightDirection = normalize(lightPosition - fragPosition);
	float diff = max(dot(normal, lightDirection), 0.0f);
	vec4 diffuse = diff * lightColour;

	// Specular
	float specularStrength = 0.5f;
	vec3 viewDirection = normalize(cameraPosition - fragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 8);
	vec4 specular = spec * specularStrength * lightColour;

	float angle = dot(spot_light_dir, -lightDirection);
	float intensity = clamp((angle - outer) / (inner - outer), 0.0f, 1.0f);

	vec4 result = texture(diffuse0, fragTexCoord) * (ambient + diffuse * intensity) + texture(specular0, fragTexCoord).r * (specular * intensity); 
	return result;
}

void main()
{
	vec3 normal = normalize(fragNormal);
	vec3 viewDirection = normalize(cameraPosition - fragPosition);

	vec3 result = cacl_dir_light(dir_light, normal, viewDirection);

	// FragColour = point_light(0.2f, 0.01f);
	FragColour = directional_light(vec3(0.0f, 1.0f, -1.0f));
	// FragColour = spot_light(0.95f, 0.90f, vec3(0.0f, -1.0f, 0.0f));
}