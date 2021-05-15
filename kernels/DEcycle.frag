vec4 colormap(float x);

uniform int iterations;
uniform float colorFrequency;

const float thickness = 0.00005f;

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
	dvec2 dc = dvec2(1, 0);
	dvec2 der = dc;
	double sqNorm = 0.;
	
	for (int i = 0; i < iterations; i++)
	{
		der = complexMultiply(der, z) * 2 + dc;
		complexSquare(z);
		z += c;
	
		sqNorm = z.x * z.x + z.y * z.y;

		if (sqNorm > 10000)
		{
			float derNorm = sqrt(float(der.x * der.x + der.y * der.y));
			float dst = log(float(sqNorm)) * sqrt(float(sqNorm)) / derNorm;
			dst = max(dst, thickness / float(zoom));
			return colormap(sin(log(1/dst)*colorFrequency) / 2.f + 0.5f);
		}
	}
	return vec4(0, 0, 0, 1);
}