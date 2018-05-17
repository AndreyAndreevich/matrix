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

    matrix[100][25] = 314;
    matrix[200][30] = 314;
    matrix[100][25] = -1;
   
    BOOST_CHECK_EQUAL(matrix[100][25],-1);
    BOOST_CHECK_EQUAL(matrix[200][30],314);
    BOOST_CHECK_EQUAL(matrix.size(),1);
  }


  BOOST_AUTO_TEST_CASE(iterator_count)
  {
    Matrix<int,-1> matrix;
    matrix[100][100] = 314;
    matrix[100][100] = -1;
    matrix[100][200] = 314;
    int count = 0;
    int x, y, v;
    for (const auto& c : matrix) {
      count++;
      std::tie(x,y,v) = c;
    }
    BOOST_CHECK_EQUAL(count,1);
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////

  BOOST_AUTO_TEST_CASE(N_matrix)
  {
    Matrix<int,0,3> matrix;
    matrix[1][2][3] = 10;
    BOOST_CHECK_EQUAL(matrix.size(),1);
  }

/////////////////////////////////////////////////////////////////////////////////////////////////////////

  BOOST_AUTO_TEST_CASE(N_matrix_assign)
  {
    Matrix<int,-1,3> matrix;
    BOOST_CHECK_EQUAL(matrix.size(),0);

    matrix[100][100][200] = 314;
    BOOST_CHECK_EQUAL(matrix[100][100][200],314);
    BOOST_CHECK_EQUAL(matrix.size(),1);
  }

/////////////////////////////////////////////////////////////////////////////////////////////////////////

  BOOST_AUTO_TEST_CASE(N_matrix_clear)
  {
    Matrix<int,-1,4> matrix;
    BOOST_CHECK_EQUAL(matrix.size(),0);

    matrix[100][100][100][100] = 314;
    matrix[200][200][200][200] = 314;
    matrix[100][100][100][100] = -1;

    BOOST_CHECK_EQUAL(matrix[100][100][100][100],-1);
    BOOST_CHECK_EQUAL(matrix[200][200][200][200],314);
    BOOST_CHECK_EQUAL(matrix.size(),1);
  }

/////////////////////////////////////////////////////////////////////////////////////////////////////////

  BOOST_AUTO_TEST_CASE(N_matrix_iterator_to_tuple)
  {
    Matrix<int,-1,5> matrix;
    matrix[1][2][3][4][5] = 314;
    int x, y, z, a, b, v;
    for (const auto& c : matrix) {
      std::tie(x,y,z,a,b,v) = c;
    }
    
    BOOST_CHECK_EQUAL(x,1);
    BOOST_CHECK_EQUAL(y,2);
    BOOST_CHECK_EQUAL(z,3);
    BOOST_CHECK_EQUAL(a,4);
    BOOST_CHECK_EQUAL(b,5);
    BOOST_CHECK_EQUAL(v,314);
  }

/////////////////////////////////////////////////////////////////////////////////////////////////////////

  BOOST_AUTO_TEST_CASE(delete_martix)
  {
    Matrix<int,-1>::element_type value;
    value = 10;
    BOOST_CHECK_EQUAL(value,10);
    {
      Matrix<int,-1> matrix;
      value = matrix[1][2] = 3;
      BOOST_CHECK_EQUAL(matrix.size(),1);
      BOOST_CHECK_EQUAL(value,3);
    }

    BOOST_CHECK_EQUAL(value,3);
    value = 5;
    BOOST_CHECK_EQUAL(value,5);
  }

BOOST_AUTO_TEST_SUITE_END()
