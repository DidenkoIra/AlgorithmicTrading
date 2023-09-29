#include "model.h"

#include <iostream>

void Model::LoadData(const std::string& filename) {
  dates_.clear();
  values_.clear();
  count_points_ = 0;
  max_value_approximation_ = 0;
  max_value_interpolation_ = 0;
  first_date_ = "";
  std::ifstream filestream(filename);
  std::string line;
  std::getline(filestream, line);
  while (std::getline(filestream, line)) {
    ReadLine(line);
    count_points_++;
  }
  ConvertDates();
  max_value_interpolation_ = max_value_approximation_;
}

void Model::ConvertDates() {
  double minutes = 60 * 60;
  dates_values_.clear();
  if (!dates_.empty()) {
    std::stringstream sstream(dates_[0]);
    std::tm date{};
    sstream >> std::get_time(&date, "%Y-%m-%d");
    first_date_ = dates_[0];
    auto first_date = std::mktime(&date);
    for (auto i = 0; i < count_points_; i++) {
      std::stringstream stream(dates_[i]);
      stream >> std::get_time(&date, "%Y-%m-%d");
      dates_values_.emplace_back(
          (std::difftime(std::mktime(&date), first_date)) / minutes);
    }
  }
}

double Model::ConvertDate(const std::string& str_date) {
  double minutes = 60 * 60;
  std::tm date{};
  std::stringstream sstream(first_date_);
  sstream >> std::get_time(&date, "%Y-%m-%d");
  auto first_date = std::mktime(&date);

  std::stringstream sstreamc(str_date);
  sstreamc >> std::get_time(&date, "%Y-%m-%d %H:%M");

  return (std::difftime(std::mktime(&date), first_date)) / minutes;
}

void Model::ReadLine(const std::string& line) {
  std::stringstream sstream(line);
  std::string token;
  std::getline(sstream, token, ',');
  ReadDate(token);
  std::getline(sstream, token, ',');
  ReadValue(token);
}

void Model::ReadValue(const std::string& str) {
  double value = std::stod(str);
  if (value > max_value_approximation_) max_value_approximation_ = value;
  values_.emplace_back(value);
}

void Model::ReadDate(const std::string& str) { dates_.emplace_back(str); }

void Model::CalcApproximation(const int polinom_degree,
                              const int count_predict_points,
                              const double to_date) {
  Approximation appr;
  double arg{0}, val{0};
  appr.CalcPolinomCoefficients(polinom_degree, dates_values_, values_);
  appr.PredictFunctionValues(count_predict_points,
                             dates_values_.back() + to_date * 24, arg, val);
  predict_values_ = appr.GetPredictFuntionValues();
  predict_dates_values_ = appr.GetPredictArgs();
  auto result =
      std::max_element(predict_values_.begin(), predict_values_.end());
  if (*result > max_value_approximation_) max_value_approximation_ = *result;
}

int Model::GetCountPoint() const { return count_points_; }

std::vector<double> const Model::GetDatesValues() { return dates_values_; }

const std::vector<double> Model::GetPredictDatesValues() {
  return predict_dates_values_;
}

const std::vector<std::string> Model::GetDates() { return dates_; }

std::vector<double> const Model::GetValues() { return values_; }

const std::vector<double> Model::GetPredictValues() { return predict_values_; }

const std::string Model::GetFirstDate() { return first_date_; }

double Model::GetMaxValueInterpolation(bool input) {
  if (input && count_points_ > 0) {
    auto result = std::max_element(values_.begin(), values_.end());
    max_value_interpolation_ = (const double)*result;
  }
  return max_value_interpolation_;
}

double Model::GetMaxValueApproximation(bool input) {
  if (input && count_points_ > 0) {
    auto result = std::max_element(values_.begin(), values_.end());
    max_value_approximation_ = (const double)*result;
  }
  return max_value_approximation_;
}

double Model::PredictValueOnDateApproximation(const int polinom_degree,
                                              const int count_predict_points,
                                              const std::string& date) {
  double result;
  Approximation appr;
  appr.CalcPolinomCoefficients(polinom_degree, dates_values_, values_);
  appr.PredictFunctionValues(count_predict_points, dates_values_.back(),
                             ConvertDate(date), result);
  return result;
}

double Model::PredictValueOnDateInterpolationSpline(const std::string& date) {
  InterpolationSpline interp;
  interp.CalcPolinomCoefficients(dates_values_, values_);
  double result = interp.PredictFunctionValue(ConvertDate(date));
  return result;
}

double Model::PredictValueOnDateInterpolationNewton(const int polinom_degree,
                                                    const std::string& date) {
  InterpolationNewton interp;
  double result = interp.PredictFunctionValue(polinom_degree, dates_values_,
                                              values_, ConvertDate(date));
  return result;
}

void Model::CalcInterpolationSpline(const int count_predict_points) {
  InterpolationSpline interp;
  interp.CalcPolinomCoefficients(dates_values_, values_);
  interp.PredictFunctionValues(count_predict_points);
  predict_values_ = interp.GetPredictFuntionValues();
  predict_dates_values_ = interp.GetPredictArgs();
  auto result =
      std::max_element(predict_values_.begin(), predict_values_.end());
  if (*result > max_value_interpolation_) max_value_interpolation_ = *result;
}

void Model::CalcInterpolationNewton(const int polinom_degree,
                                    const int count_predict_points) {
  InterpolationNewton interp;
  interp.PredictFunctionValues(polinom_degree, dates_values_, values_,
                               count_predict_points);
  predict_values_ = interp.GetPredictFuntionValues();
  predict_dates_values_ = interp.GetPredictArgs();
  auto result =
      std::max_element(predict_values_.begin(), predict_values_.end());
  if (*result > max_value_interpolation_) max_value_interpolation_ = *result;
}
