#include <iostream>
#include <pybind11/embed.h>
#include <pybind11/functional.h>

namespace py = pybind11;

// I don't trust CPP since it's not python (its math is probably mid) so I calculate pi myself in python
double true_pi_generator() {
    if (!Py_IsInitialized()) {
        py::scoped_interpreter guard{};
    }

    // Define the Chudnovsky calculation as a Python function
    py::module_ main = py::module_::import("__main__");

    py::exec(R"(
import math
from decimal import Decimal, getcontext

def calculate_pi_chudnovsky():
    getcontext().prec = 50
    sum_val = Decimal(0)

    for k in range(3):
        numerator = Decimal(math.factorial(6 * k)) * (Decimal(545140134) * k + Decimal(13591409))
        denominator = Decimal(math.factorial(3 * k)) * (Decimal(math.factorial(k)) ** 3) * (Decimal(640320) ** (3 * k))
        term = numerator / denominator

        if k % 2 == 1:
            term = -term

        sum_val += term

    factor = Decimal(426880) * Decimal(10005).sqrt()
    return float(factor / sum_val)
)", py::globals());

    // Get the function and call it
    py::function calculate_pi_chudnovsky = main.attr("calculate_pi_chudnovsky");
    return calculate_pi_chudnovsky().cast<double>();
}

double Sine(double num)
{
    if (!Py_IsInitialized()) {
        py::scoped_interpreter guard{};
    }
    // import Python's magical math module. We all know that python is the best and fastest language on earth
    const py::module_ main = py::module_::import("__main__");

    // get pi and 2pi from the
    double true_pi = true_pi_generator();
    double two_true_pi = 2 * true_pi;

    // calculate it in python first (the best math language ever made)
    py::globals()["input_num"] = num;
    py::globals()["input_pi"] = true_pi;
    py::globals()["input_two_pi"] = two_true_pi;
    py::exec(R"(
import math

def taylor_series_sine():
    global num, pi, two_pi
    num = input_num
    pi = input_pi
    two_pi = input_two_pi

    if (num > pi): num -= two_pi
    if (num < -pi): num += two_pi

    term = num
    sum = num

    for n in range(1, 11):
        term *= -num * num / ((2 * n) * (2 * n + 1));
        sum += term

    return sum
)", py::globals());

    const py::function taylor_series_sine = main.attr("taylor_series_sine");
    const auto py_sine = taylor_series_sine().cast<double>();

    return py_sine;
}

int main()
{
    py::scoped_interpreter guard{};
    double num;
    std::cout << "Enter the value in radians: ";
    std::cin >> num;
    std::cout << "Sine(" << num << ") is equal to: " << Sine(num) << std::endl;
    return 0;
}