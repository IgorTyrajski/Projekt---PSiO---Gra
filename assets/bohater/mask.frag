uniform vec2 lightCenter;   // pozycja gracza
uniform float lightRadius;  // promień światła
uniform vec2 resolution;    // rozdzielczość okna

void main()
{
    // Pozycja aktualnego fragmentu (piksela) względem lewego górnego rogu okna
    vec2 pos = vec2(gl_FragCoord.x, resolution.y - gl_FragCoord.y);

    // Oblicz odległość od światła (gracza)
    float dist = distance(pos, lightCenter);

    // Oblicz jasność: 1.0 w centrum, 0.0 poza zasięgiem światła
    float brightness = 1.0 - smoothstep(0.0, lightRadius, dist);

    // Kolor czarny, z przezroczystością odwrotnie proporcjonalną do jasności
    // (im dalej, tym mniej przezroczysty, czyli bardziej czarny)
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0 - brightness);
}
