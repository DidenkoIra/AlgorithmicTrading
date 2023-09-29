#include "interpolation_spline.h"

void InterpolationSpline::CalcPolinomCoefficients(
    const std::vector<double> &args,
    const std::vector<double> &function_values) {
  count_points_ = args.size();
  args_ = args;
  function_values_ = function_values;
  polinom_coefficients_.resize(count_points_);
  for (auto i = 0; i < count_points_; i++) {
    polinom_coefficients_[i].resize(4);
  }

  for (auto i = 0; i < count_points_ - 1; i++) {
    polinom_coefficients_[i][0] = function_values_[i];
  }
  polinom_coefficients_[0][2] = 0;
  double a{0}, f{0}, c{0};
  std::vector<double> alpha(count_points_ - 1, 0);
  std::vector<double> beta(count_points_ - 1, 0);
  for (auto i = 1; i < count_points_ - 1; i++) {
    double b, z;
    auto h_i = args_[i] - args_[i - 1];
    auto h_i1 = args_[i + 1] - args_[i];
    a = h_i;
    c = 2. * (h_i + h_i1);
    b = h_i1;
    f = 6. * ((function_values_[i + 1] - function_values_[i]) / h_i1 -
              (function_values_[i] - function_values_[i - 1]) / h_i);
    z = a * alpha[i - 1] + c;
    alpha[i] = -b / z;
    beta[i] = (f - a * beta[i - 1]) / z;
  }
  polinom_coefficients_[count_points_ - 1][2] =
      (f - a * beta[count_points_ - 2]) / (c + a * alpha[count_points_ - 2]);

  for (auto i = count_points_ - 2; i > 0; i--) {
    polinom_coefficients_[i][2] =
        alpha[i] * polinom_coefficients_[i + 1][2] + beta[i];
  }

  for (auto i = count_points_ - 1; i > 0; i--) {
    auto h_i = args_[i] - args_[i - 1];
    polinom_coefficients_[i][3] =
        (polinom_coefficients_[i][2] - polinom_coefficients_[i - 1][2]) / h_i;
    polinom_coefficients_[i][1] =
        h_i *
            (2. * polinom_coefficients_[i][2] +
             polinom_coefficients_[i - 1][2]) /
            6. +
        (function_values_[i] - function_values_[i - 1]) / h_i;
  }
}

double InterpolationSpline::CalcPredictValue(const double arg, const int k) {
  double res = 0;
  res =
      polinom_coefficients_[k][0] +
      (polinom_coefficients_[k][1] + (polinom_coefficients_[k][2] / 2. +
                                      polinom_coefficients_[k][3] * arg / 6.) *
                                         arg) *
          arg;
  return res;
}

void InterpolationSpline::EmplacePredictValue(const double arg, const int k) {
  predict_args_.emplace_back(arg);
  predict_function_values_.emplace_back(CalcPredictValue(arg - args_[k], k));
}

int InterpolationSpline::FindIndex(const double arg) {
  int index = 0;
  if (arg <= args_[0]) {
    index = 0;
  } else if (arg > args_[count_points_ - 2]) {
    index = count_points_ - 2;
  } else {
    for (auto i = count_points_ - 2; !index && i < count_points_; i--) {
      if (arg >= args_[i]) index = i;
    }
  }
  return index;
}

std::vector<double> InterpolationSpline::GetPredictArgs() const {
  return predict_args_;
}

std::vector<double> InterpolationSpline::GetPredictFuntionValues() const {
  return predict_function_values_;
}

void InterpolationSpline::PredictFunctionValues(const int count_point) {
  predict_args_.clear();
  predict_function_values_.clear();
  auto begin_arg = args_.front();
  auto end_arg = args_.back();
  auto step = (end_arg - begin_arg) / (count_point - 1);
  auto current_arg = begin_arg;
  EmplacePredictValue(begin_arg, 0);
  auto k = 1;
  for (auto i = 1; i < count_point - 1; i++) {
    current_arg += step;
    if (current_arg >= args_[k + 1]) {
      k++;
    }
    EmplacePredictValue(current_arg, k);
  }
  EmplacePredictValue(end_arg, count_points_ - 2);
}

double InterpolationSpline::PredictFunctionValue(const double arg) {
  int k = FindIndex(arg);
  double result = CalcPredictValue(arg - args_[k], k);
  return result;
}
