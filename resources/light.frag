// credit: https://github.com/xSnapi/Shadow-Casting/blob/master/Shadow%20Casting/shadow.frag

#version 330 core

precision mediump float;

layout(location = 0) out vec4 color;

in vec2 i_tex;

uniform sampler2D texture;

uniform vec2 u_entity;
uniform vec2 u_resolution;

void main() {
	vec2 uv = gl_FragCoord.xy / u_resolution;
	vec2 entity = u_entity / u_resolution;

	entity.y = 1.0 - entity.y;

	vec4 pixel = texture2D(texture, i_tex);

	float circle = 1.0 - length(uv - entity) * 4.0;

	color = vec4(vec3(0.2), circle * pixel.b * 1.0);
};
