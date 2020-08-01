attribute vec2 vPosition;
uniform mat4 uProj;
uniform mat4 uCam;

void main() {
  gl_Position = uProj * uCam * vec4(vPosition, 0.0, 1.0);
}