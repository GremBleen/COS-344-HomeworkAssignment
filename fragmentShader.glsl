#version 330 core

// Ouput data
// out vec4 color;
// in vec4 fragmentColor;
// in vec3 fragmentPos;
// in vec3 ambientColour;
// in vec3 normal;
// uniform vec3 lightPos;
// uniform vec3 lightColour;

in vec2 fragTexCoords;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void main()
{
	// float ambientStrength = 0.1;
	// vec4 ambient = vec4(ambientStrength * ambientColour, 1.0);

	// // Calculate normal in world space
	// vec3 norm = normalize(normal);
	// vec3 lightDir = normalize(lightPos - fragmentPos);

	// float diff = max(dot(norm, lightDir), 0.0);
	// vec4 diffuse = vec4(diff * lightColour, 1.0);

	// vec4 result = (ambient + diffuse) * fragmentColor;

	// color = result;

	FragColor = texture(texture_diffuse1, fragTexCoords);
}