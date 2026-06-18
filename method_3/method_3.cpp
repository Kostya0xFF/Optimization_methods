#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<functional>

using namespace std;

class Point {
private:
    vector<double> coordinates;
public:
    Point(vector<double> coordinates) : coordinates(coordinates) {}
    Point() {};

    Point operator + (const Point& other) const {
        vector<double> result;
        for (size_t i = 0; i < coordinates.size(); i++) {
            result.push_back(coordinates[i] + other[i]);
        }
        return Point(result);
    }

    Point operator - (const Point& other) const {
        vector<double> result;
        for (size_t i = 0; i < coordinates.size(); i++) {
            result.push_back(coordinates[i] - other[i]);
        }
        return Point(result);
    }

    Point operator * (double c) const {
        vector<double> result;
        for (size_t i = 0; i < coordinates.size(); i++) {
            result.push_back(coordinates[i] * c);
        }

        return Point(result);
    }

    double operator [] (int i) const {
        return coordinates[i];
    }

    double norma() const {
        double sum = 0;
        for (double x : coordinates) {
            sum += x * x;
        }
        return sqrt(sum);
    }
};

double one_dimensional_method(double a, double b, double eps, double alpha, function <double(double)> f)
{
    double a1 = a;
    double b1 = b;
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

double f(const Point& x) {
    double x1 = x[0];
    double x2 = x[1];
    return x1 * x1 + x2 + 1 + pow(x2 - x1 + 1, 4) + exp(x2);
}

Point grad(const Point& x) {
    double x1 = x[0];
    double x2 = x[1];
    double df_dx1 = 2 * x1 - 4 * pow(x2 - x1 + 1, 3);
    double df_dx2 = 1 + 4 * pow(x2 - x1 + 1, 3) + exp(x2);
    return Point({ df_dx1, df_dx2 });
}

Point grad_method(const Point& x0, double alpha, double eps,
    const function<double(const Point&)>& f,
    const function<Point(const Point&)>& grad) {
    Point x = x0;
    Point gradient;
    int k = 0;

    do {
        gradient = grad(x);
        double grad_norm = gradient.norma();

        auto F = [&f, &x, &gradient](double alpha) {
            return f(x - gradient * alpha);
            };

        double alpha_k = one_dimensional_method(0, alpha, eps, 0.01 / 3, F);

        x = x - gradient * alpha_k;
        k++;
    }

    while (gradient.norma() > eps);
    cout << "Количество итераций: " << k << ": f(x) = " << f(x) << endl;
    return x;
}


int main()
{
    Point x0({ 0.0, 0.0 });
    double eps = 0.01;
    double alpha = 1.0;
    Point result = grad_method(x0, alpha, eps, f, grad);
    cout << "Точка минимума: (" << result[0] << ", " << result[1] << ")" << endl;
}