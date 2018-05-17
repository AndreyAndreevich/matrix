#include <map>
#include <tuple>
#include <algorithm>
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
  friend U;
  friend Position<U,P,2>;

  U* matrix;
  P position;

  Position<U,P,1>(U* matrix_, P position_) : matrix(matrix_), position(position_) {}
public: 
  auto operator[](const size_t& index){
    std::get<std::tuple_size<typename U::key_type>::value-1>(position) = index;
    return matrix->getElement(position);
  }
};

template<typename U,typename P,size_t N>
class Position {
  using position_type = Position<U,P,N-1>;
  friend Position<U,P,N+1>;
  friend U;

  U* matrix;
  P position;

  Position<U,P,N>(U* matrix_, P position_) : matrix(matrix_), position(position_) {}
  
public:
  position_type operator[](const size_t& index){
    std::get<std::tuple_size<typename U::key_type>::value-N>(position) = index;
    return position_type(matrix,position);
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
  matrix_type* matrix = nullptr;

  Element<T,default_value,N>(const T& value_, const key_type& key_, matrix_type* matrix_) 
    : value(value_), key(key_), matrix(matrix_) {}

public: 
  Element<T,default_value,N>() = default;
  Element<T,default_value,N>& operator=(const T& value_) {
    value = value_;
    if (matrix)
      matrix->setElement(key,value);
    return *this;
  }

  operator T() {
    return value;
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
  using position_type = Position<matrix_type,key_type,N-1>;
  using data_type = std::map<key_type,T>;

  friend Position<matrix_type,key_type,1>;
  friend element_type;
  
  data_type data;

  element_type getElement(const key_type& key) {
    if (data.end() == data.find(key)) {
      return element_type(default_value,key,this);
    }
    return element_type(data[key],key,this);
  }

  template<typename U>
  class _iterator : public std::iterator<std::input_iterator_tag,U> {
  public:
      _iterator(U first) {
          current = first;
      }

      _iterator &operator++() {
        current++;
        return *this;
      }

      bool operator!=(const _iterator &it) {
        return current != it.current;
      }

      auto operator*() {
        return std::tuple_cat(current->first,std::make_tuple(current->second));
      }
  private:
    U current;
  };

  using iterator = _iterator<decltype(data.begin())>;
public:
  ~Matrix<T,default_value,N>() {
    //auto v = const_cast<Matrix<T,default_value,N>*>(this);

  }
  position_type operator[](const size_t& index){
    key_type key;
    std::get<0>(key) = index;
    return position_type(this,key);
  }

  auto size() {
    return data.size();
  }

  void setElement(const key_type& key, const T& value) {
    if (value == default_value)
      data.erase(key);
    else data[key] = value;
  }

  iterator begin() {
    return iterator(data.begin());
  }

  iterator end() {
    return iterator(data.end());
  }
};
//----------</Матрица>-------------------------------------------------------------------