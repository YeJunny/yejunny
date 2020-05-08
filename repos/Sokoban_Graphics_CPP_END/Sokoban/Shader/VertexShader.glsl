#version 330 core

in vec3 posAttribute;
in vec2 textureAttribute;
out vec2 passTextureAttribute;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(posAttribute, 1.0f);
    passTextureAttribute = textureAttribute;
}