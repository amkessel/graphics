#include "./project/kutils.h"
#include <float.h>

#define CORONA_SPACING 0.05

using namespace kutils;

void TestVertex()
{
	for (double ph=-90;ph<90;ph+=5)
	{
		double rad = KUTILS_COS(ph);
		double circum = 2*PI*rad;
		double arc = (circum > CORONA_SPACING) ? CORONA_SPACING : circum;
		double angle = 360*arc / circum;
		for (double th=0;th<=360+angle;th+=angle)
		{
			/*
			point3 p1 = calc_vertex(th,  ph);
			point3 p2 = calc_vertex(th+5,ph);
			point3 p3 = calc_vertex(th+5,ph+5);
			point3 p4 = calc_vertex(th,  ph+5);
			*/
			printf("(ph: %2f th: %5f) r: %5f c: %3.3f arc: %3.3f angle: %3.3f\n", ph, th,rad,circum,arc,angle);
		}
	}
}

void TestInvertHue()
{
	double hue = 55.6;
	printf("%g => %g\n", hue, InvertHue(hue));
	
	hue = 235.6;
	printf("%g => %g\n", hue, InvertHue(hue));
}

void TestFloatMax()
{
	printf("FLT_MIN = %g\n", FLT_MIN);
	printf("FLT_MAX = %g\n", FLT_MAX);
	printf("DBL_MIN = %g\n", DBL_MIN);
	printf("DBL_MAX = %g\n", DBL_MAX);
}

void TestVector3()
{
	double x1 = 1.0;
	double y1 = 3.0;
	double z1 = 2.0;
	double x2 = -1.0;
	double y2 = -3.0;
	double z2 = -7.0;

	Vector3 v1 = kutils::Vector3(x1,y1,z1);
	Vector3 v2 = kutils::Vector3(x2,y2,z2);
	
	printf("v1 = {%f, %f, %f}\n", v1.Get_x(), v1.Get_y(), v1.Get_z());
	printf("v2 = {%f, %f, %f}\n", v2.Get_x(), v2.Get_y(), v2.Get_z());
	
	double dot = v1.Dot_Product(&v2);
	printf("dot = %f\n", dot);
	
	double angle_rad = v1.Angle(&v2, true, false);
	printf("angle_rad = %g\n", angle_rad);
	
	v1.Normalize();
	v2.Normalize();
	
	printf("v1 = {%f, %f, %f}\n", v1.Get_x(), v1.Get_y(), v1.Get_z());
	printf("v2 = {%f, %f, %f}\n", v2.Get_x(), v2.Get_y(), v2.Get_z());
	
	dot = v1.Dot_Product(&v2);	
	printf("dot = %f\n", dot);
	
	angle_rad = v1.Angle(&v2, true, true);
	printf("angle_rad = %g\n", angle_rad);
}

int main(int argc, char *argv[])
{	
//	TestFloatMax();
//	TestVector3();
//	TestInvertHue();
	TestVertex();
}
