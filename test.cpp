#define BOOST_TEST_MODULE test_main
#include <boost/test/unit_test.hpp>

#include "matrix.h"

BOOST_AUTO_TEST_SUITE(matrix_test)

  BOOST_AUTO_TEST_CASE(first_initialization)
  {
    Matrix<int, -1> matrix;
    BOOST_CHECK_EQUAL(matrix.size(),0);

    auto a = matrix[0][0];
    BOOST_CHECK_EQUAL(a,-1);
    BOOST_CHECK_EQUAL(matrix.size(),0);
  }

/////////////////////////////////////////////////////////////////////////////////////////////////////////

  BOOST_AUTO_TEST_CASE(assign)
  {
    Matrix<int, -1> matrix;
    BOOST_CHECK_EQUAL(matrix.size(),0);

    matrix[100][100] = 314;
    BOOST_CHECK_EQUAL(matrix[100][100],314);
    BOOST_CHECK_EQUAL(matrix.size(),1);
  }

/////////////////////////////////////////////////////////////////////////////////////////////////////////

  BOOST_AUTO_TEST_CASE(clear)
  {
    Matrix<int, -1> matrix;
    BOOST_CHECK_EQUAL(matrix.size(),0);

    matrix[100][100] = 314;
    matrix[200][200] = 314;
    matrix[100][100] = -1;
    
    BOOST_CHECK_EQUAL(matrix[100][100],-1);
    BOOST_CHECK_EQUAL(matrix[200][200],314);
    BOOST_CHECK_EQUAL(matrix.size(),1);
  }

/////////////////////////////////////////////////////////////////////////////////////////////////////////

  BOOST_AUTO_TEST_CASE(iterator_to_tuple)
  {
    Matrix<int,-1> matrix;
    matrix[100][100] = 314;
    int x, y, v;
    for (auto& c : matrix) {
      std::tie(x,y,v) = c;
    }
    
    BOOST_CHECK_EQUAL(x,100);
    BOOST_CHECK_EQUAL(y,100);
    BOOST_CHECK_EQUAL(v,314);
  }

/////////////////////////////////////////////////////////////////////////////////////////////////////////

  BOOST_AUTO_TEST_CASE(optional)
  {
    Matrix<int,0> matrix;
    ((matrix[100][100] = 314) = 0) = 217;
   
    BOOST_CHECK_EQUAL(matrix[100][100],217);
    BOOST_CHECK_EQUAL(matrix.size(),1);
  }

/////////////////////////////////////////////////////////////////////////////////////////////////////////

  BOOST_AUTO_TEST_CASE(generate)
  {
    Matrix<int,0> matrix;

    for (auto i = 0; i < 10; i++) {
      matrix[i][i] = i;
      matrix[i][9-i] = 9 - i;
    }
    
    BOOST_CHECK_EQUAL(matrix.size(),18);
  }

BOOST_AUTO_TEST_SUITE_END()