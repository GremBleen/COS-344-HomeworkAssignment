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

#define NUM_POINT_LIGHTS 20

in vec3 fragPosition;
in vec3 fragNormal;
in vec4 fragColour;
in vec2 fragTexCoord;

out vec4 FragColour;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec3 cameraPosition;
uniform vec3 lightColour;

uniform dir_light dirLight;
uniform point_light pointLights[NUM_POINT_LIGHTS];
uniform spot_light spotLight;

vec3 cacl_dir_light(dir_light light, vec3 normal, vec3 viewDir)
{
	vec3 lightDirection = normalize(-light.direction);

	float diff = max(dot(normal, lightDirection), 0.0);

	vec3 reflectDir = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

	vec3 ambient = light.ambient * vec3(texture(diffuse0, fragTexCoord)) ;
	vec3 diffuse = light.diffuse * diff * vec3(texture(diffuse0, fragTexCoord)) ;
	vec3 specular = light.specular * spec * vec3(texture(specular0, fragTexCoord)).r;
	return (ambient + diffuse + specular);
}

vec3 cacl_point_light(point_light light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * vec3(texture(diffuse0, fragTexCoord)) ;
	vec3 diffuse = light.diffuse * diff * vec3(texture(diffuse0, fragTexCoord)) ;
	vec3 specular = light.specular * spec * vec3(texture(specular0, fragTexCoord)).r;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec3 cacl_spot_light(spot_light light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 ambient = light.ambient * vec3(texture(diffuse0, fragTexCoord)) ;
	vec3 diffuse = light.diffuse * diff * vec3(texture(diffuse0, fragTexCoord)) ;
	vec3 specular = light.specular * spec * vec3(texture(specular0, fragTexCoord)).r;
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}

void main()
{
	vec3 normal = normalize(fragNormal);
	vec3 viewDirection = normalize(cameraPosition - fragPosition);

	vec3 result = cacl_dir_light(dirLight, normal, viewDirection);

	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		result += cacl_point_light(pointLights[i], normal, viewDirection, fragPosition);
	}
	result += cacl_spot_light(spotLight, normal, viewDirection, fragPosition);
	
	FragColour = vec4(result, 1.0) * fragColour * vec4(lightColour, 1.0);
}