uniform vec2 lightCenter;
uniform float lightRadius;
uniform vec2 resolution;

void main()
{
    // Przekształcenie współrzędnych z GLSL (0,0 dolny lewy) na SFML (0,0 górny lewy)
    vec2 pos = vec2(gl_FragCoord.x, resolution.y - gl_FragCoord.y);

    float dist = distance(pos, lightCenter);
    float brightness = 1.0 - smoothstep(0.0, lightRadius, dist);
    vec4 darkness = vec4(0.0, 0.0, 0.0, 1.0);
    
    gl_FragColor = mix(darkness, vec4(0.0), brightness);
}
