#version 330 core
layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_TextureCoords;
layout (location = 2) in vec3 in_Normal;

out vec2 pass_TextureCoords;
out vec3 pass_Normal;

uniform float aspectRatio;

uniform float translateX, translateY;
uniform float cameraX, cameraY;
void main(){
    float scale = 0.02f;
    pass_TextureCoords = in_TextureCoords/16;
    pass_Normal = in_Normal;
    gl_Position = vec4(scale*(in_Position.x+translateX-cameraX)*aspectRatio, scale*(in_Position.y+translateY-cameraY), scale*in_Position.z*(1.0+scale*in_Position.z), 1.0f+scale*in_Position.z);
}
