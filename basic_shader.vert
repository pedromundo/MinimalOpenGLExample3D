#version 120
attribute vec3 aPosition;
attribute vec4 aColor;

varying vec4 vColor;

uniform mat4 MVP;

void main() {
  gl_PointSize = 10.0;
  gl_Position = MVP * vec4(aPosition,1);
  vColor = aColor;
}