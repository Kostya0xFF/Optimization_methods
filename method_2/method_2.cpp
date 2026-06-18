#include <iostream>
#include <cmath>
#include <functional>

using namespace std;

double f(double x)
{
	return cos(x - 1) + abs(x);
}

double method_1(double a, double b, double eps, double alpha, function <double(double)> f)
{
	double a1 = a, b1 = b;
	while (b1 - a1 > eps)
	{
		double xk = (a1 + b1) / 2 - alpha;
		double yk = (a1 + b1) / 2 + alpha;

		if (f(xk) > f(yk))
			a1 = xk;
		else
			b1 = yk;
	}

	return (a1 + b1) / 2;
}

double method_2(double a, double b, double eps, function <double(double)> f)
{
	double a1 = a, b1 = b;
	double c1 = (a1 + b1) / 2;
	double fc1 = f(c1);
	while (b1 - a1 > eps)
	{
		double xk = a1 + (b1 - a1) / 4;
		double yk = b1 - (b1 - a1) / 4;

		if (f(xk) < fc1)
		{
			b1 = c1;
			c1 = xk;
			fc1 = f(xk);
		}
		else if (f(yk) < fc1)
		{
			a1 = c1;
			c1 = yk;
			fc1 = f(yk);
		}
		else
		{
			a1 = xk;
			b1 = yk;
		}
	}
	return c1;
}

int main()
{
	double a = -2, b = 2, eps = 0.01, alpha = 0.01 / 3;
	cout << method_1(a, b, eps, alpha, f) << endl;
	cout << method_2(a, b, eps, f) << endl;
}