const std::string shader_f_texture = R"(
#version 430 core

uniform sampler2D u_flatTexture;
uniform bool u_blend = true;
uniform vec3 u_cameraPos;
uniform vec3 u_lightPos = vec3(0.0f, 10.0f, 0.0f);
uniform float u_ambientStrength = 0.4f;
uniform float u_specularStrength = 0.5f;

flat in vec4 vs_color;
in vec2 vs_tcoords;
in vec3 vs_fragPos;
out vec4 color;

void main() {
    if (u_blend) {
        vec3 normal = vec3(0.0, 1.0, 0.0); // hardcodet, chessboard, må fikses
        vec3 lightColor = vec3(0.5f, 0.5f, 0.5f);

        // Ambient illumination
        vec3 ambient = u_ambientStrength * lightColor;

        // Diffuse illumination
        vec3 lightDirection = normalize(u_lightPos - vs_fragPos);
        float diffuseIntensity = max(dot(normal, lightDirection), 0.0);
        vec3 diffuse = diffuseIntensity * lightColor;

        // Specular illumination
        vec3 viewDirection = normalize(u_cameraPos - vs_fragPos);
        vec3 reflectDirection = reflect(-lightDirection, normal);
        float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), 12);
        float specularIntensity = u_specularStrength * specularFactor;
        vec3 specular = specularIntensity * lightColor;

        vec4 frag = mix(vs_color, texture(u_flatTexture, vs_tcoords), 0.8);
        color = vec4(ambient + diffuse * 3.0f + specular, 1.0f) * frag;
    } else {
        color = vs_color;
    }
}
)";