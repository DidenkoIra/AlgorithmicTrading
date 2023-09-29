#pragma once
#include "../Model/model.h"

class Controller {
 public:
  explicit Controller(Model* model) : model_(model){};
  void LoadFile(const std::string& filename);
  void CalcApproximation(const int polinom_degree,
                         const int count_predict_points, const double to_date);
  void CalcInterpolationSpline(const int count_predict_points);
  void CalcInterpolationNewton(const int polinom_degree,
                               const int count_predict_points);
  int GetCountPoint() const;
  double GetMaxValueApproximation(bool input) const;
  double GetMaxValueInterpolation(bool input) const;
  double PredictValueOnDateApproximation(const int polinom_degree,
                                         const int count_predict_points,
                                         const std::string& date);
  double PredictValueOnDateInterpolationSpline(const std::string& date);
  double PredictValueOnDateInterpolationNewton(const int polinom_degree,
                                               const std::string& date);
  std::vector<double> GetDatesValues() const;
  std::vector<double> GetValues() const;
  std::vector<std::string> GetDates() const;
  std::vector<double> GetPredictDatesValues() const;
  std::vector<double> GetPredictValues() const;
  std::string const GetFirstDate();

 private:
  Model* model_;
};
