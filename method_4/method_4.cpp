#include <iostream>
#include <cmath> 
#include <vector> 
#include <functional> 
using namespace std;

double f(double x)
{
    return cos(x - 1) + fabs(x);
}

double GoldenRatio(double a, double b, double eps, function<double(double)>f) {
    double Tau = (sqrt(5) - 1) / 2;
    double a1 = a;
    double b1 = b;

    double x1 = a1 + (1 - Tau) * (b1 - a1);
    double y1 = a1 + Tau * (b1 - a1);
    double fx = f(x1);
    double fy = f(y1);

    while (b1 - a1 > eps) {

        if (fx > fy)
        {
            a1 = x1;
            x1 = y1;
            fx = fy;
            y1 = a1 + Tau * (b1 - a1);
            fy = f(y1);
        }
        else
        {
            b1 = y1;
            y1 = x1;
            fy = fx;
            x1 = a1 + (1 - Tau) * (b1 - a1);
            fx = f(x1);
        }
    }
    return (a1 + b1) / 2;
}


double FibonacciMethod(double a, double b, double eps, double alpha, function<double(double)> f)
{
    double a1 = a;
    double b1 = b;
    vector<double> Fib;
    Fib.resize(2, 1);
    int n = 1;

    while (true)
    {
        double next = Fib.back();
        if (eps >= (b1 - a1) / next) {
            break;
        }
        Fib.push_back(Fib[n] + Fib[n - 1]);
        n++;
    }

    int k = 1;

    double x1 = a1 + (Fib[n - k - 1] / Fib[n - k + 1]) * (b1 - a1);
    double y1 = a1 + (Fib[n - k] / Fib[n - k + 1]) * (b1 - a1);
    double fx = f(x1);
    double fy = f(y1);

    while (k < n - 1) {

        if (fx > fy)
        {
            a1 = x1;
            x1 = y1;
            fx = fy;
            y1 = a1 + (Fib[n - k - 1] / Fib[n - k]) * (b1 - a1);
            fy = f(y1);
        }

        else
        {
            b1 = y1;
            y1 = x1;
            fy = fx;
            x1 = a1 + (Fib[n - k - 2] / Fib[n - k]) * (b1 - a1);
            fx = f(x1);
        }
        k++;
    }

    double xn = x1;
    double yn = y1 + alpha;
    double fxn = f(xn);
    double fyn = f(yn);

    if (fxn > fyn) {
        a1 = xn;
    }
    else {
        b1 = yn;
    }

    return (a1 + b1) / 2;
}

int main() {
    cout << "Метод золотого сечения: " << GoldenRatio(-2, 2, 0.01, f) << '\n';
    cout << "Метод Фибоначчи: " << FibonacciMethod(-2, 2, 0.01, 0.001, f) << '\n';
    return 0;
}