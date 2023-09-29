#include "interpolation_newton.h"

std::vector<double> InterpolationNewton::GetPredictArgs() const {
  return predict_args_;
}

std::vector<double> InterpolationNewton::GetPredictFuntionValues() const {
  return predict_function_values_;
}

void InterpolationNewton::CalcDividedDiff(const int begin) {
  auto size = polinom_degree_ + 1;
  divivided_difs_.resize(size);
  for (auto i = 0; i < size; i++) {
    divivided_difs_[i].resize(size + 1);
  }
  for (auto row = 0; row < size; row++) {
    divivided_difs_[row][0] = args_[begin + row];
    divivided_difs_[row][1] = function_values_[begin + row];
  }
  for (auto cols = 1; cols < size + 1; cols++) {
    for (auto row = 0; row < size - cols; row++) {
      double dy = divivided_difs_[row + 1][cols] - divivided_difs_[row][cols];
      double dx = divivided_difs_[row + cols][0] - divivided_difs_[row][0];
      divivided_difs_[row][cols + 1] = dy / dx;
    }
  }
}

void InterpolationNewton::PredictFunctionValues(
    const int polinom_degree, const std::vector<double> &args,
    const std::vector<double> &function_values, const int count_point) {
  count_points_ = args.size();
  args_ = args;
  function_values_ = function_values;
  polinom_degree_ = polinom_degree;
  predict_args_.clear();
  predict_function_values_.clear();
  auto begin_arg = args_.front();
  auto end_arg = args_.back();
  auto step = (end_arg - begin_arg) / (count_point - 1);
  auto current_arg = begin_arg;
  int begin = 0;
  CalcDividedDiff(begin);
  EmplacePredictValue(begin_arg, 0);
  for (auto i = 1; i < count_point - 1; i++) {
    current_arg += step;
    if (current_arg > args_[begin + polinom_degree_]) {
      begin += polinom_degree_;
      CalcDividedDiff(begin);
    }
    EmplacePredictValue(current_arg, begin);
  }
  begin = count_points_ - polinom_degree_;
  CalcDividedDiff(begin);
  EmplacePredictValue(end_arg, begin);
}

double InterpolationNewton::PredictFunctionValue(
    const int polinom_degree, const std::vector<double> &args,
    const std::vector<double> &function_values, const double arg) {
  count_points_ = args.size();
  args_ = args;
  function_values_ = function_values;
  polinom_degree_ = polinom_degree;
  auto begin = FindIndex(arg);
  CalcDividedDiff(begin);
  return CalcPredictValue(arg, begin);
}

void InterpolationNewton::EmplacePredictValue(const double arg,
                                              const int begin) {
  predict_args_.emplace_back(arg);
  predict_function_values_.emplace_back(CalcPredictValue(arg, begin));
}

double InterpolationNewton::CalcPredictValue(const double arg,
                                             const int begin) {
  double mul = 1;
  double result = divivided_difs_[0][1];
  for (auto i = 0; i < polinom_degree_; i++) {
    mul *= (arg - args_[begin + i]);
    result += mul * divivided_difs_[0][i + 2];
  }
  return result;
}

int InterpolationNewton::FindIndex(const double arg) {
  int index = 0;
  if (arg <= args_[0]) {
    index = 0;
  } else if (arg > args_[count_points_ - polinom_degree_ - 1]) {
    index = count_points_ - polinom_degree_;
  } else {
    for (auto i = count_points_ - polinom_degree_ - 2;
         !index && i < count_points_; i--) {
      if (arg >= args_[i]) index = i;
    }
  }
  return index;
}
