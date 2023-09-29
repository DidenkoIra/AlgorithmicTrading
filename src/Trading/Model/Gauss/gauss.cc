//
// Created by Emery Reva on 10/10/22.
//

#include "gauss.h"

auto SLESolver::SolveMatrix() -> std::vector<double> {
  auto matrix = matrix_;
  int line_size{static_cast<int>(matrix[0].size()) - 1};
  SwapMatrix(matrix, line_size);
  GaussElimination(matrix, line_size);
  return Substitution(matrix, line_size);
}

auto SLESolver::SolveMatrix(const std::vector<std::vector<double>> &matrix)
    -> std::vector<double> {
  matrix_ = matrix;
  return SolveMatrix();
}

auto SLESolver::SwapMatrix(std::vector<std::vector<double>> &matrix,
                           const int &line_size) -> void {
  for (int current_row = 0; current_row < line_size; current_row++) {
    for (int current_column = current_row + 1; current_column < line_size;
         current_column++) {
      if (abs(matrix[current_row][current_row]) <
          abs(matrix[current_column][current_row])) {
        for (int k = 0; k < line_size + 1; k++) {
          matrix[current_row][k] =
              matrix[current_row][k] + matrix[current_column][k];
          matrix[current_column][k] =
              matrix[current_row][k] - matrix[current_column][k];
          matrix[current_row][k] =
              matrix[current_row][k] - matrix[current_column][k];
        }
      }
    }
  }
}

auto SLESolver::GaussElimination(std::vector<std::vector<double>> &matrix,
                                 const int &line_size) -> void {
  CoefficientRowsMultiplier(matrix, line_size, 0, matrix_.size() - 1);
}

auto SLESolver::CoefficientRowsMultiplier(
    std::vector<std::vector<double>> &matrix, const int &line_size,
    const int &row_begin, const int &row_end) -> void {
  for (int current_row = row_begin; current_row < row_end; current_row++) {
    for (int current_column = current_row + 1; current_column < line_size;
         current_column++) {
      CoefficientMultiplier(matrix, line_size, current_row, current_column);
    }
  }
}

auto SLESolver::CoefficientMultiplier(std::vector<std::vector<double>> &matrix,
                                      const int &line_size,
                                      const int &current_row,
                                      const int &current_column) -> void {
  double coefficient =
      matrix[current_column][current_row] / matrix[current_row][current_row];
  for (int k = 0; k < line_size + 1; k++) {
    matrix[current_column][k] =
        matrix[current_column][k] - coefficient * matrix[current_row][k];
  }
}

auto SLESolver::Substitution(const std::vector<std::vector<double>> &matrix,
                             const int &line_size) -> std::vector<double> {
  std::vector<double> result(line_size);
  for (int current_row = line_size - 1; current_row >= 0; current_row--) {
    result[current_row] = matrix[current_row][line_size];
    for (int current_column = current_row + 1; current_column < line_size;
         current_column++) {
      if (current_row != current_column) {
        result[current_row] =
            result[current_row] -
            matrix[current_row][current_column] * result[current_column];
      }
    }
    result[current_row] =
        result[current_row] / matrix[current_row][current_row];
  }
  return result;
}

auto SLESolver::LoadMatrix(const std::string &file_) -> void {
  std::ifstream open{file_};
  if (open.is_open()) {
    matrix_.clear();
    std::string line{};
    int lineNumber{}, general_size{}, line_size{};
    while (std::getline(open, line)) {
      matrix_.emplace_back(*new std::vector<double>);
      std::istringstream vector{line};
      while (!vector.eof()) {
        double value{};
        vector >> value;
        matrix_[lineNumber].emplace_back(value);
        line_size++;
      }
      if (!general_size) {
        general_size = line_size;
      } else if (line_size != general_size) {
        std::__throw_length_error("Матрица в файле несимметричная");
      }
      lineNumber++;
      line_size = 0;
    }
  } else {
    std::__throw_invalid_argument("Такого файла не существует!");
  }
}
