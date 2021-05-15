vec4 colormap(float x);

uniform int iterations;
uniform float colorFrequency;
uniform dvec2 juliaC;

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

vec4 getColor(dvec2 z)
{
	double sqNorm = 0.;

	for (int i = 0; i < iterations; i++)
	{
		complexSquare(z);
		z += juliaC;

		sqNorm = z.x * z.x + z.y * z.y;

		if (sqNorm > 10000)
		{
			float smoothIterCount = abs(i - log2(log2(sqrt(float(sqNorm)))));

			return colormap(abs(sin(log(smoothIterCount) * colorFrequency) / 2.f + 0.5f));
		}
	}
	return vec4(0, 0, 0, 1);
}