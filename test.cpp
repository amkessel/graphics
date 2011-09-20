#include "./HW2/kutils.h"
#include <float.h>

using namespace kutils;

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
	TestInvertHue();
}
