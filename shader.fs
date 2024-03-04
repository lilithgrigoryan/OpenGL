#version 330

out vec4 FragColor;
in vec4 Color;

const vec4 colors[6] = vec4[6]( vec4(1.0f, 0.0f, 0.0f, 1.0f),
                                vec4(0.2f, 0.7f, 0.4f, 1.0f),
                                vec4(0.0f, 0.0f, 1.0f, 1.0f),
                                vec4(1.0f, 0.6f, 0.0f, 1.0f),
                                vec4(0.9f, 0.5f, 0.9f, 1.0f),
                                vec4(0.4f, 0.3f, 0.8f, 1.0f));

void main()
{
    FragColor = Color;
}
