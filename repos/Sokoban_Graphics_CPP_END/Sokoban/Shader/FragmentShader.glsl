#version 330 core

in vec2 passTextureAttribute;
out vec4 fragmentColor;

uniform sampler2D tex;

void main()
{
    vec4 texColor = texture(tex, passTextureAttribute);
    if (texColor.a < 0.1)
    {
        discard;
    }
    fragmentColor = texColor;
}