#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec3 Color;

uniform mat4 gTransformMatrix;

out vec4 Color0;
out vec3 Normal0;
out vec3 LocalPos0;

void main()
{
    gl_Position = gTransformMatrix * vec4(Position, 1.0);
    Color0 = vec4(Color, 1.0);
    Normal0 = Normal;
    LocalPos0 = Position;
}
