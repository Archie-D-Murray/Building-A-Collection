#version 330
in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform float worldDistance;
uniform vec2 worldCentre;
uniform float time;

float random(vec2 position);

float pixelate(float value, float pixelSize);

vec2 pixelate(vec2 value, float pixelSize);

const float pixelSize = 16.0 * 4; // size of “steps” in pixels

void main() {
    float dist = distance(worldCentre, gl_FragCoord.xy);

    if (dist > worldDistance) {
        finalColor = vec4(0.2, 0.2, 0.2, 1.0); // black
    } else {
        vec4 texelColor = texture(texture0, fragTexCoord);

        // NOTE: Implement here your fragment shader code

        // final color is the color from the texture
        //    times the tint color (colDiffuse)
        //    times the fragment color (interpolated vertex color)
        finalColor = texelColor * colDiffuse * fragColor;
    }
}

float random(vec2 position) {
    return abs(fract(sin(dot(position.xy * time, vec2(12.9898, 78.233))) * 43758.5453123));
}

float pixelate(float value, float pixelSize) {
    return float(int(value * pixelSize) / int(pixelSize));
}

float getStart(float value, float increment) {
    if (value < 0) {
        return -1 * (1 + abs(ceil(value / increment)));
    } else {
        return floor(value / increment);
    }
};
