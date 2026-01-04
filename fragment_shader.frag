// fragment_shader.frag
#version 120

uniform sampler2D texture;
uniform bool hasTexture;
uniform vec2 lightPos; // Posisi Player

void main()
{
    //segelap apa area gelap
    vec4 ambient = vec4(0.3, 0.3, 0.3, 1.0);
    
    //cahaya player
    vec2 posCahaya = (gl_ModelViewProjectionMatrix * vec4(lightPos, 0, 1)).xy;
    
    //jarak pixel dari pemain
    vec2 distanceVec = posCahaya - gl_FragCoord.xy;
    distanceVec.y = distanceVec.y / 1.5; 
    float dist = length(distanceVec);

    //radius cahaya
    float attenuation = 1.0 / (1.0 + (dist / 300.0) * (dist / 300.0));

    //vignette
    vec2 uv = gl_TexCoord[0].xy;
    float distCenter = distance(uv, vec2(0.5, 0.5));
    float vignette = smoothstep(0.8, 0.4, distCenter); 
    //hasil
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    if(hasTexture == true)
    {
        //cahaya karakter + cahaya area gelap
        vec4 finalLight = ambient + vec4(attenuation, attenuation, attenuation, 1.0);
        
        //batasi nilai RGB
        if(finalLight.r > 1.0) finalLight.r = 1.0;
        if(finalLight.g > 1.0) finalLight.g = 1.0;
        if(finalLight.b > 1.0) finalLight.b = 1.0;

        //ke Piksel
        gl_FragColor = pixel * finalLight * vignette;
    }
    else
    {
        gl_FragColor = gl_Color;
    }
}