uniform vec2 windowSize;
uniform double zoom;
uniform dvec2 centerPos;
uniform int quality;

vec4 getColor(dvec2 c);

void main (void)
{
	double pxSize = 1 / zoom / windowSize.y;
	double increm = pxSize / quality;

	double hZoom = zoom * windowSize.y / windowSize.x;
	dvec2 c = dvec2(
		(gl_FragCoord.x / windowSize.x - 0.5) / hZoom + centerPos.x - pxSize / 2,
		(gl_FragCoord.y / windowSize.y - 0.5) / zoom  + centerPos.y - pxSize / 2
	);

	vec4 avgColor = vec4(0, 0, 0, 0);
	for (int i = 1; i <= quality; i++)
	{
		for (int j = 1; j <= quality; j++)
		{
			avgColor += getColor(c + dvec2(increm * i, increm * j)) / (quality * quality);
		}
	}

	gl_FragColor = avgColor;
	
}   