const std::string shader_f = R"(
#version 430 core
#define MAX_SPOTLIGHTS 10

uniform samplerCube u_cubeTexture;
uniform sampler2D u_flatTexture;

uniform vec3 u_walls_normal;
uniform bool u_illuminate = false;
uniform bool u_skybox = false;
uniform bool u_blend = false;
uniform bool u_cubemap;
uniform vec3 u_cameraPos;
uniform float u_ambientStrength = 1.0f;
uniform float u_specularStrength = 1.0f;
uniform float u_diffuseStrength = 1.0f;
uniform bool u_walls = false;

uniform vec4 u_color;
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
    vec4 frag;
    if (u_skybox) {
        color = texture(u_cubeTexture, vs_position);
        return;
    }
    if (u_blend) {
        if (u_cubemap) {
            frag = mix(u_color, texture(u_cubeTexture, vs_position), 0.5);
        } else {
            frag = mix(vs_color, texture(u_flatTexture, vs_tcoords), 0.9);
        }
    } else {
        if (u_cubemap) {
            frag = u_color;
        } else {
            frag = vs_color;
        }
    }

    if (u_illuminate) {
        vec3 normal = vs_normal;
        if (u_walls) {
            normal = u_walls_normal;
        }
        vec3 viewDir = normalize(u_cameraPos - vs_fragPos);
        vec3 lighting = vec3(0.0, 0.0, 0.0);
        for (int i = 0; i < u_numOfSpotlights; i++) {
            lighting += calcPointLight(u_spotLights[i], normal, vs_fragPos, viewDir);
        }
        frag = vec4(lighting, 1.0) * frag;
    }

    color = frag;

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
