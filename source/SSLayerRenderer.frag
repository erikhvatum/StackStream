uniform sampler2D tex;
uniform vec2 viewportSize;
uniform float rescaleMin;
uniform float rescaleRange;
uniform float gamma;
uniform vec4 tint;

vec4 rgbaTransform(vec4 rgba)
{
    vec4 rgba_out;
    rgba_out.a = rgba.a;
    vec3 gamma_rgb = vec3(gamma, gamma, gamma);
    rgba_out.rgb = pow(clamp((rgba.rgb - rescaleMin) / rescaleRange, 0.0, 1.0), gamma_rgb);
    rgba_out.rgba *= tint;
    return clamp(rgba_out, 0.0, 1.0);
}

void main()
{
    vec2 texCoord = gl_FragCoord.xy / gl_FragCoord.w;
    texCoord /= viewportSize;
    texCoord.y = 1.0 - texCoord.y;
    gl_FragColor = rgbaTransform(texture2D(tex, texCoord));
}
