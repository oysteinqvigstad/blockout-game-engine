const std::string shader_f_cubemap = R"(
#version 430 core
uniform samplerCube u_cubeTexture;
uniform vec3 u_color;

uniform vec3 u_cameraPos;
uniform vec3 u_lightPos = vec3(0.0f, 10.0f, 0.0f);
uniform bool u_blend = true;
uniform float u_ambientStrength = 1.0f;
uniform float u_specularStrength = 0.05f;

in vec3 vs_position;
in vec3 vs_normal;
in vec3 vs_fragPos;
flat in vec4 vs_color;
out vec4 color;

void main() {
    if (u_blend) {
        vec3 lightColor = vec3(0.5f, 0.5f, 0.5f);

        // Ambient illumination
        vec3 ambient = u_ambientStrength * lightColor;

        // Diffuse illumination
        vec3 lightDirection = normalize(u_lightPos - vs_fragPos);
        float diffuseIntensity = max(dot(vs_normal, lightDirection), 0.0);
        vec3 diffuse = diffuseIntensity * lightColor;

        // Specular illumination
        vec3 viewDirection = normalize(u_cameraPos - vs_fragPos);
        vec3 reflectDirection = reflect(-lightDirection, vs_normal);
        float specFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), 12);
        float specularIntensity = u_specularStrength * specFactor;
        vec3 specular = specularIntensity * lightColor;

        vec4 lighting = vec4((ambient + diffuse + specular) * u_color, 1.0);
        color = mix(lighting, texture(u_cubeTexture, vs_position), 0.4);

    } else {
        color = vec4(u_color, 1.0);
    }
}
)";