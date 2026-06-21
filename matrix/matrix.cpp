#include <iostream>
#include <vector>

using namespace std;

#define zero(n,m) Matrix<double>::zero_matrix(n,m);
#define identity(n) Matrix<double>::identity_matrix(n);

template<typename T>
class Matrix {
    size_t rows, cols;
    vector<T> array;
public:
    Matrix(size_t m, size_t n) : rows(m), cols(n), array(m*n) {}
    Matrix() {};

    static Matrix<T> zero_matrix(size_t m, size_t n) {
        return Matrix(n, m);
    }

    static Matrix<T> identity_matrix(size_t n) {
        Matrix<T> matrix_(n, n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    matrix_(i, j) = 1;
                }
            }
        }
        return matrix_;
    }

    const T& operator()(size_t i, size_t j) const {
        return array[i * cols + j];
    }

    T& operator()(size_t i, size_t j) {
        return array[i * cols + j];
    }

    size_t getRows() const {
        return rows;
    }

    size_t getCols() const{
        return cols;
    }

    Matrix<T> operator + (const Matrix<T> other) const {
        if (rows != other.rows || cols != other.cols) {
            throw invalid_argument("Ошибка! Размеры матрицы должны совпадать.");
        }
        Matrix matrix_(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                matrix_(i, j) = (*this)(i, j) + other(i, j);
            }
        }
        return matrix_;
    }

    Matrix<T> operator - (const Matrix<T> other) const {
        if (rows != other.rows || cols != other.cols) {
            throw invalid_argument("Ошибка! Размеры матрицы должны совпадать.");
        }
        Matrix matrix_(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                matrix_(i, j) = (*this)(i, j) - other(i, j);
            }
        }
        return matrix_;
    }

    Matrix<T> operator * (const Matrix<T> other) const {
        if (cols != other.rows) {
            throw std::invalid_argument("Ошибка размерности матриц.");
        }
        size_t n = cols;
        size_t m = rows;
        size_t l = other.cols;
        Matrix matrix_(m, l);
        for (size_t i = 0; i < m; i++) {
            for (size_t j = 0; j < l; j++) {
                for (size_t k = 0; k < n; k++) {
                    matrix_(i, j) += (*this)(i, k) * other(k, j);
                }
            }
        }
        return matrix_;
    }

    Matrix<T> operator * (double const_) const {
        Matrix matrix_(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                matrix_(i, j) = (*this)(i, j) * const_;
            }
        }
        return matrix_;
    }
};

template<typename T>
void print_matrix(Matrix<T>& matrix, size_t m, size_t n) {
    for (int i = 0; i < m; i++) {
        cout << "[";
        for (int j = 0; j < n; j++) {
            cout << matrix(i, j);
            if (j != n - 1) {
                cout << ' ';
            }
        }
        cout << "]";
        cout << '\n';
    }
}

template<typename T>
void print_matrix(Matrix<T>& matrix) {

    size_t n = matrix.getCols();
    size_t m = matrix.getRows();

    for (int i = 0; i < m; i++) {
        cout << "[";
        for (int j = 0; j < n; j++) {
            cout << matrix(i, j);
            if (j != n - 1) {
                cout << ' ';
            }
        }
        cout << "]";
        cout << '\n';
    }
}

int main() {
    Matrix<double> A(2, 2);
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(1, 0) = 3;
    A(1, 1) = 4;
    Matrix<double> B(2, 2);
    B(0, 0) = 2;
    B(0, 1) = 0;
    B(1, 0) = 1;
    B(1, 1) = 3;
    double const_ = 1.5;
    Matrix<double> result_0 = A * const_;
    Matrix<double> result_1 = A + B;
    Matrix<double> result_2 = A - B;
    Matrix<double> result_3 = A * B;
    print_matrix(A, 2, 2);
    cout << '\n';
    print_matrix(B, 2, 2);
    cout << '\n';
    print_matrix(result_0, 2, 2);
    cout << '\n';
    print_matrix(result_1, 2, 2);
    cout << '\n';
    print_matrix(result_2, 2, 2);
    cout << '\n';
    print_matrix(result_3, 2, 2);
    cout << '\n';
    Matrix<double> matrix_1 = zero(5,5);
    print_matrix(matrix_1, 5, 5);
    cout << '\n';
    Matrix<double> matrix_2 = identity(5);
    print_matrix(matrix_2, 5, 5);
    cout << '\n';
    Matrix<double> C(2, 3);
    C(0, 0) = 1;   
    C(0, 1) = 0;   
    C(0, 2) = 2;
    C(1, 0) = -1;  
    C(1, 1) = 3;   
    C(1, 2) = 1;
    Matrix<double> D(3, 2);
    D(0, 0) = 4;   
    D(0, 1) = 1;
    D(1, 0) = 2;   
    D(1, 1) = 0;
    D(2, 0) = -1;  
    D(2, 1) = 3;
    Matrix<double> result = C * D;
    print_matrix(result);
    return 0;
}