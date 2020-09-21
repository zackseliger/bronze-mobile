precision mediump float;

uniform sampler2D uTextureUnit;
varying vec2 vTextureCoordinates;
uniform vec4 uColor;

void main() {
  gl_FragColor = texture2D(uTextureUnit, vTextureCoordinates) + uColor;
}
