#include <set>
#include <tuple>
#include <algorithm>

//----------<сравнение кортежей в set>------------------------------------------------------

template<int N,typename ...Args>
struct compare;

template<typename ...Args>
struct compare<0,Args...> {
  bool operator()(const std::tuple<Args...>& left, const std::tuple<Args...>& right) {
    return std::get<0>(left) ==  std::get<0>(right);
  }
};

template<int N,typename ...Args>
struct compare {
  bool operator()(const std::tuple<Args...>& left, const std::tuple<Args...>& right) {
    return std::get<N>(left) ==  std::get<N>(right) ? compare<N-1,Args...>{}(left,right) : false;
  }
};

//----------</сравнение кортежей в set>------------------------------------------------------

//----------<Матрица>------------------------------------------------------

template<typename T, T default_value,int N,int current,typename ...Args>
class Matrix;

//----------<Одномерная матрица>------------------------------------------------------

template<typename T, T default_value, int N,typename ...Args>
class Matrix<T,default_value,N,1,Args...> {
  using data_type = std::tuple<Args...,int,T>;
  std::set<data_type> data;
  data_type buffer;
public:
  Matrix<T,default_value,N,1,Args...> () {
    std::get<std::tuple_size<data_type>::value - 1>(buffer) = default_value;
  }
  T& operator[](const int n){
    this->set_index<sizeof...(Args)>(n);
    auto resultIter = find(buffer);
    if (resultIter != data.end())
      buffer = *resultIter;
    else 
      std::get<std::tuple_size<data_type>::value - 1>(buffer) = default_value;
    return std::get<std::tuple_size<data_type>::value - 1>(buffer);
  }

  int size() {
    return data.size();
  }

  template<int M>
  void set_index(int index) {
    std::get<M>(buffer) = index;
  }

  auto begin() {
    return data.begin();
  }

  auto end() {
    return data.end();
  }

  void check(const int&) {
    if(std::get<std::tuple_size<data_type>::value - 1>(buffer) != default_value) {
      data.insert(buffer);
    } else {
      auto iter = find(buffer);
      if (iter != data.end()) {
        data.erase(iter);
      }
    }
  }

private:
  auto find(const data_type& element) -> decltype(data.begin()) {
    return find_if(data.begin(),data.end(),
      [element,this](data_type data){
        return this->m_compare(element,data);;
      });
  }

  bool m_compare(const data_type& left, const data_type& right) {
    return compare<sizeof...(Args),Args...,int,T>{}(left,right);
  }
};

//----------</Одномерная матрица>------------------------------------------------------

//----------<N-мерная матрица>------------------------------------------------------

template<typename T, T default_value,int N = 2,int current = N,typename ...Args>
class Matrix {
  using matrix = Matrix<T,default_value,N,current-1,Args...,int>;
  matrix inserted;
public:
  matrix& operator[](const int n){
    this->check(current);
    this->set_index<sizeof...(Args)>(n);
    return inserted;
  }

  int size() {
    this->check(current);
    return inserted.size();
  }

  template<int M>
  void set_index(int index) {
    inserted.set_index<M>(index);
  }

  void check(const int& current_) {
    if (current_ != N)
      return;
    inserted.check(current_);
  }

  auto begin() {
    check(current);
    return inserted.begin();
  }

  auto end() {
    return inserted.end();
  }
};

//----------<N-мерная матрица>------------------------------------------------------

//----------</Матрица>------------------------------------------------------
