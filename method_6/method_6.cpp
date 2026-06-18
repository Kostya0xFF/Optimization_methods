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

Matrix hesse_matrix(const Point& x) {
    Matrix hesse;
    double x_1 = x[0];
    double x_2 = x[1];

    double a_11 = 2 + 12 * (x_2 - x_1 + 1) * (x_2 - x_1 + 1);
    double a_12 = -12 * (x_2 - x_1 + 1) * (x_2 - x_1 + 1);
    double a_21 = -12 * (x_2 - x_1 + 1) * (x_2 - x_1 + 1);
    double a_22 = 12 * (x_2 - x_1 + 1) * (x_2 - x_1 + 1) + exp(x_2);

    hesse = {
        Point({a_11, a_12}),
        Point({a_21, a_22})
    };

    return hesse;
}

Point multiply_matrix(const Matrix& A, const Point& x) {
    vector<double> result(A.size(), 0.0);

    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < x.size(); j++) {
            result[i] += A[i][j] * x[j];
        }
    }
    return Point(result);
}


Matrix inverse_matrix(const Matrix& A) {
    Matrix inverse;
    double det_a = A[0][0] * A[1][1] - A[0][1] * A[1][0];

    if (det_a == 0) {
        throw invalid_argument("Определитель равен нулю!");
    }

    double a_11 = A[1][1] / det_a;
    double a_12 = -1 * A[0][1] / det_a;
    double a_21 = -1 * A[1][0] / det_a;
    double a_22 = A[0][0] / det_a;

    inverse = {
        Point({a_11, a_12}),
        Point({a_21, a_22})
    };

    return inverse;
}

Point Newton_Raphson(const Point& x_0, double eps, function<double(Point)> f) {
    int k = 0;
    Point x = x_0;

    do {
        Matrix H = hesse_matrix(x);
        Point mult_grad_inverse_H = multiply_matrix(inverse_matrix(H), grad(x));

        auto F = [&](double alpha) {
            return f(x - mult_grad_inverse_H * alpha);

            };

        double alpha_k = goldenSectionSearch(F, 0, 1, eps);
        x = x - mult_grad_inverse_H * alpha_k;
        k++;
    } while (grad(x).norma() > eps);

    cout << "Число итераций: " << k << '\n';
    return x;
}


int main()

{
    Point x_0({ 0.0, 0.0 });
    double eps = 0.01;
    Point result = Newton_Raphson(x_0, eps, f);
    cout << "Точка минимума функции: " << result.toString();
    return 0;
}