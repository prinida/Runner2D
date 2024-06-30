#version 430

in vec2 tc;

out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

uniform sampler2D samp;

void main(void)
{ 
    vec4 texColor = texture(samp, tc);

    if(texColor.a <= 0.01f)
        discard;

    color = texColor;
}