#version 330 core
layout(location=0) out vec4 out_Color;

in vec2 pass_TextureCoords;
in vec3 pass_Normal;
uniform sampler2D tiles;
void main(){
    vec3 color = texture(tiles, pass_TextureCoords).rgb;
    float lightIntensity = dot(pass_Normal, normalize(vec3(1.0f,2.0f,-3.0f)));
    if(lightIntensity<0.1f)
        lightIntensity = 0.1f;
    out_Color = vec4(lightIntensity*color, 1.0f);
}

