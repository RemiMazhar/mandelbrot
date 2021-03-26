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

bool isInside(dvec2 c, out float valueOutput)
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
			float smoothIterCount = abs(i - log2(log2(sqrt(float(sqNorm)))));

			valueOutput = abs(sin(log(smoothIterCount) * colorFrequency) / 2.f + 0.5f);

			return false;
		}
	}
	return true;
}