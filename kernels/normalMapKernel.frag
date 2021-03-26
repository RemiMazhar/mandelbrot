uniform int iterations;
uniform float colorFrequency;

const float lightAngle = 45;
const double lightHeight = 1;

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

dvec2 complexDivide(dvec2 v1, dvec2 v2)
{
	dvec2 res;
	double a = v1.x;
	double b = v1.y;
	double c = v2.x;
	double d = v2.y;
	res.x = (a * c + b * d) / (c * c + d * d); 
	res.y = (b * c - a * d) / (c * c + d * d);
	return res;
}

bool isInside(dvec2 c, out float valueOutput)
{
	dvec2 v = dvec2(cos(radians(lightAngle)), sin(radians(lightAngle)));

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
			dvec2 u = complexDivide(z, der);
			u = u / length(u);
			double t = dot(u, v) + lightHeight;
			t = t / (1 + lightHeight);
			valueOutput = float(t);
			return false;
		}
	}
	valueOutput = 0;
	return true;
}