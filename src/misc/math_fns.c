

/*  Mathematical routine which calculates J1(2*PI*x)/x  accurate to
	a part in 1e8 fairly quickly.  J1 is first order Bessel function
	of the first kind. PI = 3.1415...
*/

double j1ox(double x)
{
	extern double fabs(), sqrt(), cos(), sin();
	double y, z, xx;
	static double  r1 = 454667714532., r2 = -1.95837112839e12,
		r3 = 2.37370267453e12, r4 = -1.14920424935e12,
		r5 = 239685878685., r6 = -18172520199.1;
	static double s1 = 144725228442., s2 = 90821488470.7, 
		s3 = 28962925170.2, s4 = 6118892052.43, 
		s5 = 915755015.705, s6 = 95895600.6162;
	static double p2 = .183105e-2, p3 = -.3516396496e-4,
		p4 = .2457520174e-5, p5 = -.240337019e-6;
	static double  q1 = .04687499995e0, q2 = -.2002690873e-3,
		q3 = .8449199096e-5, q4 = -.88228987e-6, q5 = .105787412e-6;
   	static double t1 = 3.2734324022940, t2 = -14.611075806204,
		t3 = 19.598764579345, t4 = -11.588593409532,
		t5 = 3.5270139466306, t6 = -0.59032483091092,
		t7 = 5.4687269385051e-2, t8 = -2.6230261444829e-3,
		t9 = 5.0784301472517e-05;
	static double u2 = 0.49500590785793,
 		u3 =  7.1339893679187e-2, u4 = 3.0749927279085e-2,
		u5 = -2.4105432215685e-3, u6 = 1.001244636654e-3,
		u7 = -7.6374586616697e-5, u8 = 6.4822218969379e-6;

	if ( (y = x*x) < 1.62113893828)
        	return ((r1+y*(r2+y*(r3+y*(r4+y*(r5+y*r6)))))
			/(s1+y*(s2+y*(s3+y*(s4+y*(s5+y*s6))))));
	else if (y < 9.)
        	return ((t1+y*(t2+y*(t3+y*(t4+y*(t5+y*(t6+y*(t7+y*(t8+
			y*t9))))))))/(1.0+y*(u2+y*(u3+y*(u4+y*(u5+y*(u6+
			y*(u7+y*u8))))))));
	else {
		x = fabs(x)*6.28318530718;
		z = 8./x;
		y = z*z;
		xx = x - 2.356194491;
		return (sqrt(0.392699081473*y/x)*(cos(xx)*(1.+y*(p2+y*(p3+y*
			(p4+y*p5))))-z*sin(xx)*(q1+y*(q2+y*(q3+y*(q4+y*q5))))));
	}
}
