//-----------------------------------------------------------------------------
//
// Online Combinatorial Optimization
// types.hpp
//
//-----------------------------------------------------------------------------

#ifndef TYPES
#define TYPES

//-----------------------------------------------------------------------------
// Imports
//-----------------------------------------------------------------------------

// STD Library
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <fstream>
#include <functional>
#include <future>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <random>
#include <regex>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

// Boost Library
// #include <boost/math/constants/constants.hpp>
// #include <boost/math/distributions/binomial.hpp>
// #include <boost/multiprecision/cpp_int.hpp>
// #include <boost/multiprecision/gmp.hpp>
// #include <boost/random/mersenne_twister.hpp>
// #include <boost/random/uniform_int_distribution.hpp>

// Armadillo Library
#include <armadillo>

// MKL Library
//#include <mkl.h>

// OMP Library
//#include <omp.h>


//-----------------------------------------------------------------------------
// POD Types
//-----------------------------------------------------------------------------

using sword = arma::sword;
using uword = arma::uword;
// using mpi = boost::multiprecision::mpz_int;
// using mpr = boost::multiprecision::mpq_rational;

// using mpf50 = boost::multiprecision::mpf_float_50;
// using mpf100 = boost::multiprecision::mpf_float_100;
// using mpf500 = boost::multiprecision::mpf_float_500;
// using mpf1000 = boost::multiprecision::mpf_float_1000;
// using mpf = boost::multiprecision::mpf_float;

using cx_float = arma::cx_float;
using cx_double = arma::cx_double;

//-----------------------------------------------------------------------------
// Arrays & Vectors Types
//-----------------------------------------------------------------------------

template<typename T> using Array = std::vector<T>;
using darray = std::vector<double>;
using farray = std::vector<float>;
using iarray = std::vector<sword>;
using uarray = std::vector<uword>;

template<typename T> using Col = arma::Col<T>;
using dvec = arma::dvec;
using fvec = arma::fvec;
using ivec = arma::ivec;
using uvec = arma::uvec;
using cx_dvec = arma::cx_dvec;
using cx_fvec = arma::cx_fvec;

template<typename T> using Row = arma::Row<T>;
using drowvec = arma::drowvec;
using frowvec = arma::frowvec;
using irowvec = arma::irowvec;
using urowvec = arma::urowvec;
using cx_drowvec = arma::cx_drowvec;
using cx_frowvec = arma::cx_frowvec;

//-----------------------------------------------------------------------------
// Matrices Types
//-----------------------------------------------------------------------------

template<typename T> using Mat = arma::Mat<T>;
using dmat = arma::dmat;
using fmat = arma::fmat;
using imat = arma::imat;
using umat = arma::umat;
using cx_dmat = arma::cx_dmat;
using cx_fmat = arma::cx_fmat;

template<typename T> using SpMat = arma::SpMat<T>;
using sp_dmat = arma::sp_dmat;
using sp_fmat = arma::sp_fmat;
using sp_imat = arma::sp_imat;
using sp_umat = arma::sp_umat;
using sp_cx_dmat = arma::sp_cx_dmat;
using sp_cx_fmat = arma::sp_cx_fmat;

//-----------------------------------------------------------------------------
// Cube Types
//-----------------------------------------------------------------------------

template<typename T> using Cube = arma::Cube<T>;
using dcube = arma::dcube;
using fcube = arma::fcube;
using icube = arma::icube;
using ucube = arma::ucube;
// using cx_dcube = arma::cx_dcube;
// using cx_fcube = arma::cx_fcube;

//-----------------------------------------------------------------------------
// Field Types
//-----------------------------------------------------------------------------

template<typename T> using Field = arma::field<T>;

//-----------------------------------------------------------------------------
// Map Types
//-----------------------------------------------------------------------------

template<typename T> using HashMap = std::unordered_map<uword, T>;

//-----------------------------------------------------------------------------
// Random generators
//-----------------------------------------------------------------------------

using mte = std::mt19937;
// using mte = boost::random::mt19937;

//-----------------------------------------------------------------------------
// Structures
//-----------------------------------------------------------------------------

using upair = std::pair<uword, uword>;
template<typename T> using uptr = std::unique_ptr<T>;
template<typename T> using init_list = std::initializer_list<T>;

#endif
