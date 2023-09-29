#include "approximation.h"

double Approximation::CalcSum(const int degree) {
  double res = 0;
  for (auto i = 0; i < count_points_; i++) {
    res += pow(args_[i], degree);
  }
  return res;
};

void Approximation::CalcFreeCoeff() {
  auto col = polinom_degree_ + 1;
  for (auto i = 0; i < polinom_degree_ + 1; i++) {
    sums_[i][col] = 0;
    for (auto point = 0; point < count_points_; point++) {
      sums_[i][col] += function_values_[point] * pow(args_[point], i);
    }
  }
}

void Approximation::CalcSums() {
  sums_.resize(polinom_degree_ + 1);
  for (auto i = 0; i < polinom_degree_ + 1; i++)
    sums_[i].resize(polinom_degree_ + 2);
  for (auto i = 0; i < polinom_degree_ + 1; i++) {
    for (auto j = 0; j < polinom_degree_ + 1; j++) {
      sums_[i][j] = CalcSum(i + j);
    }
  }
  CalcFreeCoeff();
}

double Approximation::CalcPredictValue(const double arg) {
  double res = polinom_coefficients_[0];
  for (auto i = 1; i <= polinom_degree_; i++) {
    res += polinom_coefficients_[i] * pow(arg, i);
  }
  return res;
}

void Approximation::CalcPolinomCoefficients(
    const int polinom_degree, const std::vector<double> &args,
    const std::vector<double> &function_values) {
  polinom_degree_ = polinom_degree;
  count_points_ = args.size();
  args_ = args;
  function_values_ = function_values;
  CalcSums();

  SLESolver gauss;
  polinom_coefficients_ = gauss.SolveMatrix(sums_);
}

void Approximation::EmplacePredictValue(const double arg) {
  predict_args_.emplace_back(arg);
  predict_function_values_.emplace_back(CalcPredictValue(arg));
}

void Approximation::PredictFunctionValues(const int count_point,
                                          const double to_arg, const double arg,
                                          double &predict) {
  predict_args_.clear();
  predict_function_values_.clear();
  auto begin_arg = args_.front();
  auto end_arg = args_.back();
  auto step = (end_arg - begin_arg) / (count_point - 1);
  auto current_arg = begin_arg;
  EmplacePredictValue(begin_arg);
  for (auto i = 1; i < count_point - 1; i++) {
    current_arg += step;
    EmplacePredictValue(current_arg);
  }
  EmplacePredictValue(end_arg);
  if (to_arg > end_arg) {
    current_arg = end_arg + step;
    while (current_arg < to_arg) {
      EmplacePredictValue(current_arg);
      current_arg += step;
    }
    EmplacePredictValue(to_arg);
  }
  predict = CalcPredictValue(arg);
}

std::vector<double> Approximation::GetPredictArgs() const {
  return predict_args_;
}

std::vector<double> Approximation::GetPredictFuntionValues() const {
  return predict_function_values_;
}
