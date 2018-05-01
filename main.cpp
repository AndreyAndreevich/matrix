#include "matrix.h"

#include <iostream>

int main() {

  Matrix<int,0> matrix;

  for (auto i = 0; i < 10; i++) {
    matrix[i][i] = i;
    matrix[i][9-i] = 9 - i;
  }

  for (auto i = 1; i < 9; i++) {
    for (auto j = 1; j < 9; j++) 
      std::cout << matrix[i][j] << " ";
    std::cout << std::endl;
  }

  std::cout << std::endl << "Matrix size : " << matrix.size() << std::endl << std::endl;

  int x,y,v;
  for (const auto &element : matrix) {
    std::tie(x,y,v) = element;
    std::cout << "Matrix[" << x << "][" << y << "] = " << v << std::endl;
  }

  return 0;
}