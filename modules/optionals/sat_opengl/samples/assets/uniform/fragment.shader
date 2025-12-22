#version 330 core

out vec4 out_color;

uniform float input_color;

void main (void)
{
    out_color = vec4 (input_color, 0.0f, 0.0f, 1.0f);
}