#version 330
in vec3 TexCoords;
out vec4 color;
uniform samplerCube diaskybox;
uniform samplerCube nocheskybox;
uniform float u_mixFactor;
void main()
{
vec4 dia = texture(diaskybox,TexCoords);
vec4 noche = texture(nocheskybox,TexCoords);
color=mix(dia, noche, u_mixFactor);
}