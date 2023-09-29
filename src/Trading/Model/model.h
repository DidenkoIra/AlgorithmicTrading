#ifndef TRADING_MODEL_MODEL_H
#define TRADING_MODEL_MODEL_H

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

#include "Approximation/approximation.h"
#include "InterpolationNewton/interpolation_newton.h"
#include "InterpolationSpline/interpolation_spline.h"

class Model {
 public:
  Model(){};
  ~Model(){};
  void LoadData(const std::string& file_name);
  void CalcApproximation(const int polinom_degree,
                         const int count_predict_points, const double to_date);
  void CalcInterpolationSpline(const int count_predict_points);
  void CalcInterpolationNewton(const int polinom_degree,
                               const int count_predict_points);
  int GetCountPoint() const;
  double GetMaxValueApproximation(bool input);
  double GetMaxValueInterpolation(bool input);
  double PredictValueOnDateApproximation(const int polinom_degree,
                                         const int count_predict_points,
                                         const std::string& date);
  double PredictValueOnDateInterpolationSpline(const std::string& date);
  double PredictValueOnDateInterpolationNewton(const int polinom_degree,
                                               const std::string& date);
  std::vector<std::string> const GetDates();
  std::vector<double> const GetDatesValues();
  std::vector<double> const GetPredictDatesValues();
  std::vector<double> const GetValues();
  std::vector<double> const GetPredictValues();
  std::string const GetFirstDate();

 private:
  std::vector<double> values_{};
  std::vector<double> predict_values_{};
  std::vector<double> dates_values_{};
  std::vector<double> predict_dates_values_{};
  std::vector<std::string> dates_{};
  std::string first_date_;
  int count_points_{0};
  int count_predict_points_{0};
  double max_value_interpolation_{0};
  double max_value_approximation_{0};
  void ReadDate(const std::string& str);
  void ReadValue(const std::string& str);
  void ReadLine(const std::string& line);
  void ConvertDates();
  double ConvertDate(const std::string& str_date);
};

#endif  // TRADING_MODEL_MODEL_H
