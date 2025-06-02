uniform vec2 lightCenter;
uniform float lightRadius;

void main()
{
    vec2 pixelPos = gl_FragCoord.xy;
    float dist = distance(pixelPos, lightCenter);

    // Zamiast przyciemniać środek, przyciemniamy na zewnątrz
    float alpha = smoothstep(lightRadius * 0.8, lightRadius, dist);
    gl_FragColor = vec4(0.0, 0.0, 0.0, alpha); // Czarne z przezroczystością
}