vec4 colormap(float x);

uniform int iterations;
uniform float colorFrequency;

const double eps = 0.3;
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

vec2 complexDivide(dvec2 v1, dvec2 v2)
{
	vec2 res;
	float a = float(v1.x);
	float b = float(v1.y);
	float c = float(v2.x);
	float d = float(v2.y);
	res.x = (a * c + b * d) / (c * c + d * d); 
	res.y = (b * c - a * d) / (c * c + d * d);
	return res;
}

vec4 getColor(dvec2 c)
{
	dvec2 z = c;
	dvec2 dc = dvec2(1, 0);
	dvec2 der = dc;
	double sqNormZ = 0.;
	float sqNormD = 0.;

	for (int i = 0; i < iterations; i++)
	{
		der = 2 * complexMultiply(z, der) + dc;

		sqNormD = float(der.x * der.x + der.y * der.y);

		complexSquare(z);
		z += c;

		sqNormZ = z.x * z.x + z.y * z.y;

		if (sqNormZ > 10000)
		{
			vec2 u = complexDivide(z, der);
			u = u / length(u);
			float t = dot(u, v) + lightHeight;
			t = t / (1 + lightHeight);
			return t * colormap(abs(sin(log(sqNormD) * colorFrequency) / 2.f + 0.5f));
		}
	}
	return vec4(0, 0, 0, 1);
}