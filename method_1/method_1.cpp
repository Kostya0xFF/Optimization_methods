#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<functional>

using namespace std;

class Point {
private:
    vector<double> coord;
public:
    Point(vector<double> coord) : coord(coord) {}
    Point() {};

    Point operator + (const Point& other) const {
        vector<double> result;
        for (size_t i = 0; i < coord.size(); i++) {
            result.push_back(coord[i] + other[i]);
        }
        return Point(result);
    }

    Point operator - (const Point& other) const {
        vector<double> result;
        for (size_t i = 0; i < coord.size(); i++) {
            result.push_back(coord[i] - other[i]);
        }
        return Point(result);
    }

    Point operator * (double c) const {
        vector<double> result;
        for (size_t i = 0; i < coord.size(); i++) {
            result.push_back(coord[i] * c);
        }
        return Point(result);
    }

    double operator [] (int i) const {
        return coord[i];
    }


    double normal() const {
        double sum = 0;
        for (double x : coord) {
            sum += x * x;
        }
        return sqrt(sum);
    }
};

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

Point grad_method(const Point& x0, double alpha, double epsilon, double gamma, double theta,
    const function<double(const Point&)>& f,
    const function<Point(const Point&)>& grad) {

    Point x = x0;
    int k = 1;
    Point gradient;
    do
    {
        gradient = grad(x);
        double grad_norm = gradient.normal();
        double ak = alpha;
        while (true) {
            if (f(x - gradient * ak) - f(x) <= -gamma * ak * grad_norm * grad_norm) {
                break;
            }
            ak *= theta;
        }
        x = x - gradient * ak;
        k++;
    } while (gradient.normal() > epsilon);
    cout << "Количество итераций: (" << k << ")" << endl;
    return x;
}


int main()
{
    Point x0({ 0.0, 0.0 });
    double epsilon = 0.01;
    double alpha = 1.0;
    double gamma = 0.5;
    double theta = 0.5;
    Point result = grad_method(x0, alpha, epsilon, gamma, theta, f, grad);
    cout << "Точка минимума: (" << result[0] << ", " << result[1] << ")" << endl;
    return 0;
}
