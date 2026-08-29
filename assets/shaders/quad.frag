#version 330 core

struct Point_Light {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Spot_Light {
    vec3 position;
    vec3 direction;
    float cutoff;
    float outer_cutoff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define MAX_POINT_LIGHTS 2
#define MAX_SPOT_LIGHTS 1

in vec4 v_color;
in vec3 v_position;
in vec3 v_normal;
in vec2 v_uv;
in float v_view_depth;

uniform vec3 u_view_position;
uniform vec3 u_global_ambient;
uniform int u_point_light_count;
uniform int u_spot_light_count;
uniform Point_Light u_point_lights[MAX_POINT_LIGHTS];
uniform Spot_Light u_spot_lights[MAX_SPOT_LIGHTS];

out vec4 color;

vec3 point_light(Point_Light light, vec3 normal, vec3 position) {
    vec3 light_direction = normalize(light.position - position);
    float diffuse = max(dot(normal, light_direction), 0.0);
    float distance_to_light = length(light.position - position);
    float attenuation = 1.0 / (
        light.constant +
        light.linear * distance_to_light +
        light.quadratic * distance_to_light * distance_to_light
    );

    return (light.ambient + light.diffuse * diffuse) * attenuation;
}

vec3 spot_light(Spot_Light light, vec3 normal, vec3 position) {
    vec3 light_direction = normalize(light.position - position);
    float diffuse = max(dot(normal, light_direction), 0.0);
    float distance_to_light = length(light.position - position);
    float attenuation = 1.0 / (
        light.constant +
        light.linear * distance_to_light +
        light.quadratic * distance_to_light * distance_to_light
    );

    float theta = dot(light_direction, normalize(-light.direction));
    float cone_width = max(light.cutoff - light.outer_cutoff, 0.0001);
    float intensity = clamp((theta - light.outer_cutoff) / cone_width, 0.0, 1.0);

    return light.ambient * attenuation +
    light.diffuse * diffuse * attenuation * intensity;
}

void main() {
    vec3 normal = normalize(v_normal);
    vec3 lighting = u_global_ambient;

    for (int i = 0; i < u_point_light_count; ++i) {
        lighting += point_light(u_point_lights[i], normal, v_position);
    }

    for (int i = 0; i < u_spot_light_count; ++i) {
        lighting += spot_light(u_spot_lights[i], normal, v_position);
    }

    color = vec4(v_color.rgb * lighting, v_color.a);
}
