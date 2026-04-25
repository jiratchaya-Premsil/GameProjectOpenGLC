#version 330 core 

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D textTexture;
uniform vec3 textColor;

void main (){
    
    vec4 tex = texture(textTexture, TexCoord);
    FragColor = vec4(textColor, tex.a);

    // OPTION B: If your font has black background (no alpha)
    // if (tex.r < 0.1 && tex.g < 0.1 && tex.b < 0.1)
    //     discard;
    // FragColor = vec4(textColor, 1.0);
}