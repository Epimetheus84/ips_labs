#define _USE_MATH_DEFINES

#include <iostream>
#include <iomanip>
#include <math.h>
#include <cmath>
#include <chrono>

using namespace std::chrono;
using namespace std;


typedef double(*pointFunc)(double);

// почему-то инлайн функция не помогла избавиться от ошибки 1200 векторизатора, 
// заменил вызов функции на ее содержимое, костыли наше всё :)
inline double f(double x) {
    return  6 / sqrt(x*(2-x));
}

double left_rectangle_integral(double a, double b, int n)
{
    double Res = 0.0;
    double h = (b - a) / n;
    double* y = new double[n];
    double x;
    int i;

    // #pragma loop(ivdep)
    // #pragma loop(hint_parallel(8))
    #pragma loop(no_vector)
    for (i = 0; i < n; i++) {
        x = a + h * i;
        y[i] = 6 / sqrt(x * (2 - x));
    }

    for (i = 0; i < n; i++) {
        Res += y[i];
    }

    return Res * h;
}

double right_rectangle_integral(double a, double b, int n)
{
    double Res = 0.0;
    double h = (b - a) / n;
    double *y = new double[n];
    double x;
    int i;
    
    // #pragma loop(ivdep)
    // #pragma loop(hint_parallel(8))
    #pragma loop(no_vector)
    for (i = 1; i <= n; i++) {
        x = a + h * i;
        y[i - 1] = 6 / sqrt(x * (2 - x));
    }

    for (i = 0; i < n; i++) {
        Res += y[i];
    }

    return Res * h;
}

int main() {
    double a = 0.5, b = 1;
    double s1;
    int n;
    int intervals[] = { 100, 1000, 10000, 100000, 1000000 };

    for (int i = 0; i < sizeof(intervals) / sizeof(int); i++) {
        n = intervals[i];
        cout << "Count of intervals = " << n << endl;

        cout << fixed;
        cout << setprecision(10);

        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        s1 = left_rectangle_integral(a, b, n);

        cout << "Left integral = " << s1 << endl;
        // Посчитаем точность в процентах, зная что при аналитическом решении ответ будет равен 1 Pi
        cout << "Accuracy (%) = " << 100 - (abs(M_PI - s1) / M_PI * 100) << endl;

        s1 = right_rectangle_integral(a, b, n);
        cout << "Right integral = " << s1 << endl;
        cout << "Accuracy (%) = " << 100 - (abs(M_PI - s1) / M_PI * 100) << endl;

        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> duration = (t2 - t1);
        cout << "Duration is: " << duration.count() << " seconds" << endl << endl;
    }

    system("pause");
}

