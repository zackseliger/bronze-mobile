attribute vec2 vPosition;
uniform mat4 uProj;
uniform mat4 uCam;

attribute vec2 aTextureCoordinates;
varying vec2 vTextureCoordinates;

void main() {
  vTextureCoordinates = aTextureCoordinates;
  gl_Position = uProj * uCam * vec4(vPosition, 0.0, 1.0);
}
