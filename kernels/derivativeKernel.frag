vec4 colormap(float x);

uniform int iterations;
uniform float colorFrequency;

const double eps = 0.3;

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

dvec2 complexMultiply(dvec2 v1, dvec2 v2)
{
	dvec2 res;
	double a = v1.x;
	double b = v1.y;
	double c = v2.x;
	double d = v2.y;
	res.x = a * c - b * d; 
	res.y = a * d + b * c;
	return res;
}

vec4 getColor(dvec2 c)
{
	dvec2 z = c;
	dvec2 der = dvec2(1, 0);
	double sqNormZ = 0.;
	float sqNormD = 0.;

	for (int i = 0; i < iterations; i++)
	{
		der = 2 * complexMultiply(z, der) + dvec2(1, 0);

		sqNormD = float(der.x * der.x + der.y * der.y);

		complexSquare(z);
		z += c;

		sqNormZ = z.x * z.x + z.y * z.y;

		if (sqNormZ > 10000)
		{
			return colormap(abs(sin(log(sqNormD) * colorFrequency) / 2.f + 0.5f));
		}
	}
	return vec4(0, 0, 0, 1);
}