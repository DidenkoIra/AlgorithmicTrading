#include "controller.h"

void Controller::LoadFile(const std::string &filename) {
  model_->LoadData(filename);
}

void Controller::CalcApproximation(const int polinom_degree,
                                   const int count_predict_points,
                                   const double to_date) {
  model_->CalcApproximation(polinom_degree, count_predict_points, to_date);
}

int Controller::GetCountPoint() const { return model_->GetCountPoint(); }

std::vector<double> Controller::GetDatesValues() const {
  return model_->GetDatesValues();
}

std::vector<double> Controller::GetValues() const {
  return model_->GetValues();
}

std::vector<std::string> Controller::GetDates() const {
  return model_->GetDates();
}

std::vector<double> Controller::GetPredictDatesValues() const {
  return model_->GetPredictDatesValues();
}

std::vector<double> Controller::GetPredictValues() const {
  return model_->GetPredictValues();
}

const std::string Controller::GetFirstDate() { return model_->GetFirstDate(); }

double Controller::GetMaxValueApproximation(bool input) const {
  return model_->GetMaxValueApproximation(input);
}

double Controller::GetMaxValueInterpolation(bool input) const {
  return model_->GetMaxValueInterpolation(input);
}

double Controller::PredictValueOnDateApproximation(
    const int polinom_degree, const int count_predict_points,
    const std::string &date) {
  return model_->PredictValueOnDateApproximation(polinom_degree,
                                                 count_predict_points, date);
}

double Controller::PredictValueOnDateInterpolationSpline(
    const std::string &date) {
  return model_->PredictValueOnDateInterpolationSpline(date);
}

double Controller::PredictValueOnDateInterpolationNewton(
    const int polinom_degree, const std::string &date) {
  return model_->PredictValueOnDateInterpolationNewton(polinom_degree, date);
}

void Controller::CalcInterpolationSpline(const int count_predict_points) {
  return model_->CalcInterpolationSpline(count_predict_points);
}

void Controller::CalcInterpolationNewton(const int polinom_degree,
                                         const int count_predict_points) {
  return model_->CalcInterpolationNewton(polinom_degree, count_predict_points);
}
