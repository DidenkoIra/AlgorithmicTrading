#ifndef TRADING_MODEL_GAUSS_GAUSS_H
#define TRADING_MODEL_GAUSS_GAUSS_H

//
// Created by Emery Reva on 10/10/22.
//

#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class SLESolver {
 public:
  auto LoadMatrix(const std::string &file_) -> void;
  auto SolveMatrix() -> std::vector<double>;
  auto SolveMatrix(const std::vector<std::vector<double>> &matrix)
      -> std::vector<double>;

 private:
  static auto SwapMatrix(std::vector<std::vector<double>> &matrix,
                         const int &lineSize) -> void;
  auto GaussElimination(std::vector<std::vector<double>> &matrix,
                        const int &lineSize) -> void;
  static auto Substitution(const std::vector<std::vector<double>> &matrix,
                           const int &lineSize) -> std::vector<double>;
  static auto CoefficientMultiplier(std::vector<std::vector<double>> &matrix,
                                    const int &lineSize, const int &currentRow,
                                    const int &currentColumn) -> void;
  static auto CoefficientRowsMultiplier(
      std::vector<std::vector<double>> &matrix, const int &lineSize,
      const int &row_begin, const int &row_end) -> void;
  std::vector<std::vector<double>> matrix_{};
};

#endif  // TRADING_MODEL_GAUSS_GAUSS_H
