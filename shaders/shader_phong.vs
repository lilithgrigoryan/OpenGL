#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

uniform mat4 gTransformMatrix;

out vec3 TexCoord0;
out vec3 Normal0;
out vec3 LocalPos0;

void main()
{
    gl_Position = gTransformMatrix * vec4(Position, 1.0);
    TexCoord0 = vec3(TexCoord, 0.5);
    Normal0 = Normal;
    LocalPos0 = Position;
}
