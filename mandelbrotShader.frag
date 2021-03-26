// declare functions that will be defined in another file
vec4 colormap(float x);
bool isInside(dvec2 c, out float valueOutput);

uniform vec2 windowSize;
uniform double zoom;
uniform dvec2 centerPos;

void main (void)
{
	double hZoom = zoom * windowSize.y / windowSize.x;
	dvec2 c = dvec2(
		(gl_FragCoord.x / windowSize.x - 0.5) / hZoom + centerPos.x,
		(gl_FragCoord.y / windowSize.y - 0.5) / zoom  + centerPos.y
	);

	float value;
	if (isInside(c, value))
	{
		gl_FragColor = vec4(0, 0, 0, 1);
	}
	else
	{
		gl_FragColor = colormap(value);
	}
	
}   