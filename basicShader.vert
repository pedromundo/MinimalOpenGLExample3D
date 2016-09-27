#version 400
in vec3 aPosition;
in vec4 aColor;
out vec4 vColor;
uniform mat4 MVP;
void main() {
    gl_Position = MVP * vec4(aPosition,1);
    vColor = aColor;
}