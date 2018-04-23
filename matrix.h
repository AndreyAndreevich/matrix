#include <set>
#include <iostream>
#include <tuple>
#include <functional>
#include <algorithm>

template<typename T, T default_value>
class Matrix;


template<typename T, T default_value>
class matrix_body {
  friend class Matrix<T,default_value>;
  std::set<std::tuple<int,int,T>> elements;
  std::tuple<int,int,T> previous_value;

  matrix_body() {
    previous_value = std::make_tuple(0,0,default_value);
  }

  void check() { 
    if(std::get<2>(previous_value) != default_value)
      elements.emplace(previous_value);
    else {
      auto iter = std::find_if(elements.begin(),elements.end(),
        [this](decltype(*elements.begin()) element){
        return std::get<0>(this->previous_value) == std::get<0>(element) 
            && std::get<1>(this->previous_value) == std::get<1>(element);
      });
      if (iter != elements.end())
        elements.erase(iter);
    }
  }

public:
  T& operator[](const size_t m){
    std::get<1>(previous_value) = m;
    auto iter = std::find_if(elements.begin(),elements.end(),
    [this](decltype(*elements.begin()) element){
      return std::get<0>(this->previous_value) == std::get<0>(element) 
          && std::get<1>(this->previous_value) == std::get<1>(element);
    });
    if (iter == elements.end()) 
      std::get<2>(previous_value) = default_value;
    else std::get<2>(previous_value) = std::get<2>(*iter);
    return std::get<2>(previous_value);
  }
};


template<typename T, T default_value>
class Matrix {
  using matrix = matrix_body<T,default_value>;

  matrix body;
public:
  size_t size() {
    body.check();
    return body.elements.size();
  }

  matrix& operator[](const size_t n) {
    body.check();
    std::get<0>(body.previous_value) = n;
    return body;
  }

  auto begin() {
    body.check();
    return body.elements.begin();
  }

  auto end() {
    body.check();
    return body.elements.end();
  }
};