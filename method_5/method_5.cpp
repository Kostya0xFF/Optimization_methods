#include<iostream>
#include<cmath>
#include<vector>
#include<functional>
#include<string>

using namespace std;

double goldenSectionSearch(function<double(double)> f, double a, double b, double eps) {
    const double golden_ratio = (sqrt(5) - 1) / 2.0;

    double x1 = b - golden_ratio * (b - a);
    double x2 = a + golden_ratio * (b - a);

    double f1 = f(x1);
    double f2 = f(x2);

    while (fabs(b - a) > eps) {
        if (f1 > f2) {
            a = x1;
            x1 = x2;
            f1 = f2;
            x2 = a + golden_ratio * (b - a);
            f2 = f(x2);
        }
        else {
            b = x2;
            x2 = x1;
            f2 = f1;
            x1 = b - golden_ratio * (b - a);
            f1 = f(x1);
        }
    }

    return (a + b) / 2;
}

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

    int size() const {
        return coordinates.size();
    }

    double operator [] (int i) const {
        return coordinates[i];
    }

    string toString() const {
        string result = "(";
        for (size_t i = 0; i < coordinates.size(); i++) {
            result += to_string(coordinates[i]);
            if (i != coordinates.size() - 1) {
                result += ", ";
            }
        }
        result += ")";
        return result;
    }

    double norma() const {
        double sum = 0;
        for (double x : coordinates) {
            sum += x * x;
        }
        return sqrt(sum);
    }
};

typedef vector<Point> Matrix;

double dot(const Point& a, const Point& b) {
    double result = 0.0;
    for (int i = 0; i < a.size(); i++) {
        result += a[i] * b[i];
    }
    return result;
}

Point multiply(const Matrix& A, const Point& x) {
    vector<double> Ax(A.size(), 0.0);
    for (int i = 0; i < A.size(); i++) {
        Ax[i] = dot(A[i], x);
    }
    return Point(Ax);
}

class sqrFunction {
private:
    Matrix A;
    Point b;
    double c;
public:
    sqrFunction(const Matrix& A, const Point& b, double c) : A(A), b(b), c(c) {}

    double operator()(const Point& x) const {
        Point Ax = multiply(A, x);
        return 0.5 * dot(x, Ax) + dot(b, x) + c;
    }

    Point gradient(const Point& x) const {
        Point Ax = multiply(A, x);
        return Ax + b;
    }

    const vector<Point>& getA() const {
        return A;
    }
};

Point Fletcher_Reeves(const sqrFunction& f, double eps, Point x_0, bool key) {
    int k = 1;

    Point x = x_0;
    Point grad = f.gradient(x);
    Point grad_prev;
    Point x_prev;
    double alpha;
    Point d;

    do {

        if (k == 1) {
            d = grad;
        }
        else {
            double norma_k = grad.norma();
            double norma_k_prev = grad_prev.norma();
            d = grad + d * (pow(norma_k, 2) / pow(norma_k_prev, 2));
        }

        if (key) {
            auto F = [&x, &d, &f](double alpha_val) {
                return f(x - d * alpha_val);
                };
            alpha = goldenSectionSearch(F, 0, 1, eps);
        }
        else {
            Point Ad = multiply(f.getA(), d);
            alpha = dot(grad, d) / dot(d, Ad);
        }

        x_prev = x;
        grad_prev = grad;

        x = x - d * alpha;
        grad = f.gradient(x);
        k++;

    } while (grad.norma() > eps);

    cout << "Число итераций: " << k - 1 << '\n';
    return x;
}

int main()
{
    Matrix A = { Point({3, -1, 1}), Point({-1, 3, -1}), Point({1, -1, 1}) };
    Point b({ 9,8,7 });
    double c = 6;
    sqrFunction f(A, b, c);
    Point x_0({ 0, 0, 0 });
    double eps = 0.01;
    cout << "========================================" << '\n';
    cout << "============= ПЕРВЫЙ МЕТОД =============" << '\n';
    cout << "========================================" << '\n';
    Point result_1 = Fletcher_Reeves(f, eps, x_0, true);
    cout << "Точка минимума: " << result_1.toString() << '\n';
    cout << "Значение функции в точке: " << f(result_1) << '\n';
    cout << "========================================" << '\n';
    cout << "============= ВТОРОЙ МЕТОД =============" << '\n';
    cout << "========================================" << '\n';
    Point result_2 = Fletcher_Reeves(f, eps, x_0, false);
    cout << "Точка минимума|: " << result_2.toString() << '\n';
    cout << "Значение функции в точке|: " << f(result_2) << '\n';
    return 0;
}