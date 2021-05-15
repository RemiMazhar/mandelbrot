vec4 colormap(float x);

uniform int iterations;
uniform float colorFrequency;
uniform dvec2 juliaC;

const float lightAngle = 45;
const float lightHeight = 1.5;
const vec2 v = vec2(cos(radians(lightAngle)), sin(radians(lightAngle)));

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
	res.x = ((a * c + b * d) / (c * c + d * d)); 
	res.y = ((b * c - a * d) / (c * c + d * d));
	return res;
}

vec4 getColor(dvec2 z)
{
	dvec2 c = juliaC;
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
			float smoothIterCount = abs(i - log2(log2(sqrt(float(sqNorm)))));

			dvec2 u = complexDivide(z, der);
			u = u / length(u);

			float t = float(dot(u, v)) + lightHeight;
			t = t / (1 + lightHeight);
			return t * colormap(abs(sin(log(smoothIterCount) * colorFrequency) / 2.f + 0.5f));
		}
	}
	return vec4(0, 0, 0, 1);
}