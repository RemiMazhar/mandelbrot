vec4 colormap(float x);

uniform int iterations;
uniform float colorFrequency;

const double thickness = 0.002;

float log_b(float x, float a)
{
	return log(x) / log(a);
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
	dvec2 der = dvec2(1, 0);
	double sqNorm = 0.;

	for (int i = 0; i < iterations; i++)
	{
		der = complexMultiply(der, z) * 2 + dvec2(1, 0);
		complexSquare(z);
		z += c;

		sqNorm = z.x * z.x + z.y * z.y;

		if (sqNorm > 10000)
		{
			der *= thickness / zoom;
			double derSqNorm = der.x * der.x + der.y * der.y;
			return sqNorm*(pow(log(float(sqNorm)), 2)) < derSqNorm ? colormap(0) : colormap(1);
		}
	}
	return colormap(1);
}