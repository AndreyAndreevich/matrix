#include <map>
#include <tuple>
#include <algorithm>
#include <memory>
#include <exception>

#include <iostream>

//----------<tuple по количеству>------------------------------------------------------
template<int COUNT,typename ...Args>
struct make_tuple_from_count;

template<typename ...Args>
struct make_tuple_from_count<1,Args...> {
  using type = std::tuple<size_t,Args...>;
};

template<int COUNT,typename ...Args>
struct make_tuple_from_count : make_tuple_from_count<COUNT-1,size_t,Args...> {};
//----------</tuple по количеству>------------------------------------------------------

//----------<класс положений в матрице>-------------------------------------------------
template<typename T, T default_value,size_t N>
class Matrix;

template<typename U,typename P,size_t N>
class Position;

template<typename U,typename P>
class Position<U,P,1> {
  using matrix_type = typename U::element_type;

  friend matrix_type;
  friend Position<U,P,2>;

  U matrix_wptr;
  mutable P position;

  Position<U,P,1>(U matrix_wptr_, P position_) : matrix_wptr(matrix_wptr_), position(position_) {}
public: 
  auto operator[](const size_t& index){
    if (matrix_wptr.expired())
      throw std::runtime_error("matrix deleted");
    std::get<std::tuple_size<typename matrix_type::key_type>::value-1>(position) = index;
    return matrix_wptr.lock()->getElement(position);
  }
};

template<typename U,typename P,size_t N>
class Position {
  using matrix_type = typename U::element_type;
  using position_type = Position<U,P,N-1>;
  friend Position<U,P,N+1>;
  friend matrix_type;

  U matrix_wptr;
  mutable P position;

  Position<U,P,N>(U matrix_wptr_, P position_) : matrix_wptr(matrix_wptr_), position(position_) {}
  
public:
  position_type operator[](const size_t& index){
    if (matrix_wptr.expired())
      throw std::runtime_error("matrix deleted");
    std::get<std::tuple_size<typename U::element_type::key_type>::value-N>(position) = index;
    return position_type(matrix_wptr,position);
  }
};
//----------</класс положений в матрице>------------------------------------------------

//----------<активный элемент матрицы>--------------------------------------------------
template<typename T, T default_value,size_t N>
class Element {
  using matrix_type = Matrix<T,default_value,N>;
  using key_type = typename make_tuple_from_count<N>::type;
  using position_type = Position<matrix_type,key_type,1>;

  friend position_type;
  friend matrix_type;

  template<typename T_, T_ default_value_,size_t N_>
  friend std::ostream& operator<<(std::ostream&, const Element<T_,default_value_,N_>&);
  template<typename T_, T_ default_value_,size_t N_>
  friend bool operator ==(const Element<T_,default_value_,N_>& left, const T_& right);

  T value = default_value;
  key_type key;
  std::weak_ptr<matrix_type> matrix_wptr;

  Element<T,default_value,N>(const key_type& key_, const T& value_, std::weak_ptr<matrix_type> matrix_wptr_) 
    : key(key_), value(value_), matrix_wptr(matrix_wptr_) {}

public: 
  Element<T,default_value,N>() = default;
  Element<T,default_value,N>& operator=(const T& value_) {
    value = value_;
    if (!matrix_wptr.expired() && !matrix_wptr.lock()->isConst)
      matrix_wptr.lock()->setElement(key,value);
    return *this;
  }
};

template<typename T_, T_ default_value_,size_t N_>
std::ostream& operator<<(std::ostream& out, const Element<T_,default_value_,N_>& element) {
  out << element.value;
  return out;
}

template<typename T_, T_ default_value_,size_t N_>
bool operator ==(const Element<T_,default_value_,N_>& left, const T_& right) {
  return left.value == right;
}
//----------</активный элемент матрицы>-------------------------------------------------

//----------<Матрица>-------------------------------------------------------------------
template<typename T, T default_value,size_t N = 2>
class Matrix {
public:
  using key_type = typename make_tuple_from_count<N>::type;
  using element_type = Element<T,default_value,N>;
private:
  using matrix_type = Matrix<T,default_value,N>;
  using position_type = Position<std::weak_ptr<matrix_type>,key_type,N>;
  using data_type = std::map<key_type,T>;

  friend Position<std::weak_ptr<matrix_type>,key_type,1>;
  friend element_type;
  
  data_type data;
  std::shared_ptr<matrix_type> thisPtr;
  mutable bool isConst = false;

  element_type getElement(const key_type& key) {
    if (data.end() == data.find(key)) {
      return element_type(key,default_value,std::weak_ptr<matrix_type>(thisPtr));
    }
    return element_type(key,data.at(key),std::weak_ptr<matrix_type>(thisPtr));
  }

  void setElement(const key_type& key, const T& value) {
    if (value == default_value)
      data.erase(key);
    else data[key] = value;
  }

  template<typename U>
  class _iterator : public U {
  public:
    _iterator(U first) : U(first) {}
    auto operator->() = delete;
    auto operator*() {
      auto current = U::operator*();
      return std::tuple_cat(current.first,std::make_tuple(current.second));
    }
  };

  using iterator = _iterator<decltype(data.begin())>;
public:
  Matrix<T,default_value,N>() {
    thisPtr = std::shared_ptr<matrix_type>(this,[](auto ptr){});
  }

  Matrix<T,default_value,N>(Matrix<T,default_value,N>& matrix) {
    data = matrix.data;
    thisPtr = std::shared_ptr<matrix_type>(this,[](auto ptr){});
  }

  ~Matrix<T,default_value,N>() {

  }

  auto operator[](const size_t& index){
    key_type key;
    std::get<0>(key) = index;
    return position_type(std::weak_ptr<matrix_type>(thisPtr),key)[index];
  }

  auto operator[](const size_t& index) const {
    isConst = true;
    key_type key;
    std::get<0>(key) = index;
    return position_type(std::weak_ptr<matrix_type>(thisPtr),key)[index];
  }

  auto size() {
    return data.size();
  }

  iterator begin() {
    return iterator(data.begin());
  }

  iterator end() {
    return iterator(data.end());
  }
};
//----------</Матрица>-------------------------------------------------------------------