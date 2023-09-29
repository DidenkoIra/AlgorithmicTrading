#ifndef TRADING_MODEL_INTERPOLATIONSPLINE_INTERPOLATION_SPLINE_H
#define TRADING_MODEL_INTERPOLATIONSPLINE_INTERPOLATION_SPLINE_H

#include <cmath>
#include <vector>

class InterpolationSpline {
 public:
  InterpolationSpline(){};
  ~InterpolationSpline(){};
  void CalcPolinomCoefficients(const std::vector<double> &args,
                               const std::vector<double> &function_values);
  void PredictFunctionValues(const int count_point);
  double PredictFunctionValue(const double arg);
  std::vector<double> GetPredictArgs() const;
  std::vector<double> GetPredictFuntionValues() const;

 private:
  std::vector<double> args_{};
  std::vector<double> function_values_{};
  std::vector<double> predict_args_{};
  std::vector<double> predict_function_values_{};
  int count_points_{};
  std::vector<std::vector<double>> polinom_coefficients_;
  double CalcPredictValue(const double arg, const int k);
  void EmplacePredictValue(const double arg, const int k);
  int FindIndex(const double arg);
};

#endif  // TRADING_MODEL_INTERPOLATIONSPLINE_INTERPOLATION_SPLINE_H
