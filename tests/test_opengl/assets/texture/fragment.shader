#version 330 core 

out vec4 FragColor;

in vec3 ourColor;
in vec2 TextCoord;

uniform sampler2D texture1;

void main ()
{
    // FragColor = texture (texture1, TextCoord) * vec4 (ourColor, 1.0);    
    FragColor = texture (texture1, TextCoord);
}