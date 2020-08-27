attribute vec2 vPosition;
uniform mat4 uProj;
uniform mat4 uCam;

attribute vec3 aColor;
varying vec3 vColor;

void main() {
  vColor = aColor;
  gl_Position = uProj * uCam * vec4(vPosition, 0.0, 1.0);
}