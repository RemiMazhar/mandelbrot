vec4 colormap(float x);

uniform int iterations;
uniform float colorFrequency;

float log_b(float x, float a)
{
	return log(x) / log(a);
}

void complexSquare(inout dvec2 v)
{
	double x = v.x;
	double y = v.y;
	v.x = x * x - y * y; 
	v.y = 2 * x * y;
}

vec4 getColor(dvec2 c)
{
	dvec2 z = c;
	double sqNorm = 0.;

	for (int i = 0; i < iterations; i++)
	{
		complexSquare(z);
		z += c;

		sqNorm = z.x * z.x + z.y * z.y;

		if (sqNorm > 10000)
		{
			return z.y < 0 ? colormap(0) : colormap(1);
		}
	}
	return colormap(1);
}