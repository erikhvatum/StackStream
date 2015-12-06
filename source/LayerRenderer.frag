uniform sampler2D tex;
uniform vec2 viewportSize;

void main()
{
    vec2 texCoord = gl_FragCoord.xy / gl_FragCoord.w;
    texCoord /= viewportSize;
    texCoord.y = 1.0f - texCoord.y;
    gl_FragColor = texture2D(tex, texCoord);
};
