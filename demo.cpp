#include "Calculator.h"

using namespace asl;

int main()
{
	const int n = 10000; // do computation n times

	double t_eval, t_compute, t_native;

	// using eval():
	{
		double y = 0;
		double t1 = now();
		for (int i = 0; i < n; i++) {
			y += Calculator::eval("-3.5*cos(i*5)+pi*(-2+sqrt(i*pi^2))/3", Dic<double>("i", i));
		}
		double t2 = now();
		t_eval = (t2 - t1) / n;
		printf("Eval:    %f (%.3f us)\n", y, t_eval * 1e6);
	}

	// parsing once and then using compute():
	{
		Calculator calc("-3.5*cos(i*5)+pi*(-2+sqrt(i*pi^2))/3");
		double y = 0;
		double t1 = now();
		for (int i = 0; i < n; i++) {
			y += calc.set("i", i).compute();
		}
		double t2 = now();
		t_compute = (t2 - t1) / n;
		printf("Compute: %f (%.3f us)\n", y, t_compute * 1e6);
	}

	// now the same in native code:
	{
		double y = 0;
		double t1 = now();
		for (int i = 0; i < n; i++)
			y += -3.5*cos(i*5.0) + PI*(-2 + sqrt(i*pow(PI, 2))) / 3;
		double t2 = now();
		t_native = (t2 - t1) / n;
		printf("Native:  %f (%.3f us)\n", y, t_native * 1e6);
	}

	printf("\nRelative times:\nnative: 1\ncompute: %.1f\neval: %.1f\n", t_compute/t_native, t_eval/t_native);
}

