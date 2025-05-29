uniform mat4 model;
uniform mat4 camMatrix;

void main() {
    gl_Position = camMatrix * model * vec4(position, 1.0);
}