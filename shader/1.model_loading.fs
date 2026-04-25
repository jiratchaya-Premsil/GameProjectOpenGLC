#version 330 core
out vec4 FragColor;

in vec2 TexCoords;


uniform sampler2D texture_diffuse1;



void main()
{
   vec4 texColor = texture(texture_diffuse1, TexCoords);
     vec3 ambientColor     = vec3(76.0, 185.0, 218.0) / 255.0;
    vec3 outdoorAmbient   = vec3(239.0, 80.0, 52.0) / 255.0;
    float brightness      = 1.2;

    vec3 color = texColor.rgb;
    color += ambientColor * 0.1;
    color += outdoorAmbient * 0.2;

     //color *= brightness;

    // Soft look
    float gray = dot(color, vec3(0.299, 0.587, 0.114));
    color = mix(vec3(gray), color, 1.2);




     FragColor = vec4(color, texColor.a);
    
}