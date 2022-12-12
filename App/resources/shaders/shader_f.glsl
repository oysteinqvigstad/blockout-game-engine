const std::string shader_f = R"(
#version 430 core
#define MAX_SPOTLIGHTS 10

uniform samplerCube u_cubeTexture;
uniform sampler2D u_flatTexture;

uniform bool u_skybox = false;
uniform bool u_blend = true;
uniform bool u_cubemap;
uniform vec3 u_cameraPos;
uniform float u_ambientStrength = 1.0f;
uniform float u_specularStrength = 1.0f;
uniform float u_diffuseStrength = 1.0f;
uniform bool u_chessBoard_normal = false;

uniform vec3 u_color;
flat in vec4 vs_color;

in vec3 vs_position;
in vec2 vs_tcoords;
in vec3 vs_normal;
in vec3 vs_fragPos;
out vec4 color;

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

uniform PointLight lights;
uniform PointLight u_spotLights[MAX_SPOTLIGHTS];
uniform int u_numOfSpotlights = 0;

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    if (u_skybox) {
        color = texture(u_cubeTexture, vs_position);
        return;
    }
    if (u_blend) {
        vec3 normal;
        vec4 frag;
        if (u_cubemap) {
            normal = vs_normal;
            frag = mix(vec4(u_color, 1.0), texture(u_cubeTexture, vs_position), 0.5);
        } else {
            normal = vs_normal;
            if (u_chessBoard_normal)
                normal = vec3(0.0, 1.0, 0.0);
            frag = mix(vs_color, texture(u_flatTexture, vs_tcoords), 0.9);
        }

        vec3 viewDir = normalize(u_cameraPos - vs_fragPos);
        vec3 lighting = vec3(0.0, 0.0, 0.0);
        for (int i = 0; i < u_numOfSpotlights; i++) {
            lighting += calcPointLight(u_spotLights[i], normal, vs_fragPos, viewDir);
        }
        color = vec4(lighting, 1.0) * frag;
    } else {
        color = vec4(u_color, 1.0);
    }
}


vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    // diffuse lighting
    vec3 lightDirection = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDirection), 0.0);

    // specular lighting
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDirection), 0.0), 12);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                               light.quadratic * (distance * distance));

    // results
    vec3 ambient = light.ambient * attenuation * u_ambientStrength;
    vec3 diffuse = light.diffuse * diff * attenuation * u_diffuseStrength;
    vec3 specular = light.specular * spec * attenuation * u_specularStrength;

    return (ambient + diffuse + specular);
}
)";
