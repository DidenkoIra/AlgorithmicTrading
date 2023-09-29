#ifndef TRADING_MODEL_APPROXIMATION_APPROXIMATION_H
#define TRADING_MODEL_APPROXIMATION_APPROXIMATION_H

#include <cmath>
#include <vector>

#include "../Gauss/gauss.h"

class Approximation {
 public:
  Approximation(){};
  ~Approximation(){};
  void CalcPolinomCoefficients(const int polinom_degree,
                               const std::vector<double> &args,
                               const std::vector<double> &function_values);
  void PredictFunctionValues(const int count_point, const double to_arg,
                             const double arg, double &predict);
  std::vector<double> GetPredictArgs() const;
  std::vector<double> GetPredictFuntionValues() const;

 private:
  std::vector<double> args_{};
  std::vector<double> function_values_{};
  std::vector<double> predict_args_{};
  std::vector<double> predict_function_values_{};
  std::vector<double> polinom_coefficients_{};
  int polinom_degree_{0};
  int count_points_{0};
  std::vector<std::vector<double>> sums_;
  double CalcSum(const int degree);
  void CalcFreeCoeff();
  void CalcSums();
  double CalcPredictValue(const double arg);
  void EmplacePredictValue(const double arg);
};

#endif  // TRADING_MODEL_APPROXIMATION_APPROXIMATION_H