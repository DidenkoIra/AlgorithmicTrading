#ifndef TRADING_MODEL_INTERPOLATIONNEWTON_INTERPOLATION_NEWTON_H
#define TRADING_MODEL_INTERPOLATIONNEWTON_INTERPOLATION_NEWTON_H

#include <vector>

class InterpolationNewton {
 public:
  InterpolationNewton(){};
  ~InterpolationNewton(){};
  void PredictFunctionValues(const int polinom_degree,
                             const std::vector<double> &args,
                             const std::vector<double> &function_values,
                             const int count_point);
  double PredictFunctionValue(const int polinom_degree,
                              const std::vector<double> &args,
                              const std::vector<double> &function_values,
                              const double arg);
  std::vector<double> GetPredictArgs() const;
  std::vector<double> GetPredictFuntionValues() const;

 private:
  std::vector<double> args_{};
  std::vector<double> function_values_{};
  std::vector<double> predict_args_{};
  std::vector<double> predict_function_values_{};
  std::vector<std::vector<double>> divivided_difs_{};
  int polinom_degree_{0};
  int count_points_{};
  void CalcDividedDiff(const int begin);
  void EmplacePredictValue(const double arg, const int begin);
  double CalcPredictValue(const double arg, const int begin);
  int FindIndex(const double arg);
};

#endif  // TRADING_MODEL_INTERPOLATIONNEWTON_INTERPOLATION_NEWTON_H
