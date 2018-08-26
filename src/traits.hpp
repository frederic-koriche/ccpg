// -----------------------------------------------------------------------------
//
// Online Combinatorial Optimization
// traits.hpp
//
// -----------------------------------------------------------------------------

#ifndef TRAITS_HPP
#define TRAITS_HPP

#include "enums.hpp"

using std::false_type;
using std::true_type;

// -----------------------------------------------------------------------------
// Traits for scalars
// -----------------------------------------------------------------------------

namespace traits
{
	using void_type = std::integral_constant<int, 000>;
	using bool_type = std::integral_constant<int, 001>;
	using uword_type = std::integral_constant<int, 002>;
	using sword_type = std::integral_constant<int, 003>;
	using float_type = std::integral_constant<int, 004>;
	using double_type = std::integral_constant<int, 005>;

	struct to_bool : public bool_type {};
	struct to_uword : public uword_type {};
	struct to_sword : public sword_type {};
	struct to_double : public double_type {};
	struct to_float : public float_type {};

	template<typename T> struct type_of : public void_type {};
	template<typename T> struct type_of<const T>: public type_of<T> {};
	template<> struct type_of<bool>: public bool_type {};
	template<> struct type_of<uword>: public uword_type {};
	template<> struct type_of<sword>: public sword_type {};
	template<> struct type_of<float>: public float_type {};
	template<> struct type_of<double>: public double_type {};

	template<typename T> struct is_bool : public false_type {};
	template<> struct is_bool<bool>: public true_type {};
	template<> struct is_bool<const bool>: public true_type {};

	template<typename T> struct is_uword : public false_type {};
	template<> struct is_uword<uword>: public true_type {};
	template<> struct is_uword<const uword>: public true_type {};

	template<typename T> struct is_sword : public false_type {};
	template<> struct is_sword<sword>: public true_type {};
	template<> struct is_sword<const sword>: public true_type {};

	template<typename T> struct is_integral : public false_type {};
	template<typename T> struct is_integral<const T>: public is_integral<T> {};
	template<> struct is_integral<bool>: public true_type {};
	template<> struct is_integral<uword>: public true_type {};
	template<> struct is_integral<sword>: public true_type {};

	template<typename T> struct is_float : public false_type {};
	template<> struct is_float<float>: public true_type {};
	template<> struct is_float<const float>: public true_type {};

	template<typename T> struct is_double : public false_type {};
	template<> struct is_double<double>: public true_type {};
	template<> struct is_double<const double>: public true_type {};

	template<typename T> struct is_decimal : public false_type {};
	template<typename T> struct is_decimal<const T>: public is_decimal<T> {};
	template<> struct is_decimal<float>: public true_type {};
	template<> struct is_decimal<double>: public true_type {};

	struct scalar : public true_type {};
	struct nonscalar : public false_type {};

	template<typename T> struct is_scalar : public nonscalar {};
	template<typename T> struct is_scalar<const T>: public is_scalar<T> {};
	template<> struct is_scalar<bool>: public scalar {};
	template<> struct is_scalar<uword>: public scalar {};
	template<> struct is_scalar<sword>: public scalar {};
	template<> struct is_scalar<float>: public scalar {};
	template<> struct is_scalar<double>: public scalar {};
}

// -----------------------------------------------------------------------------
// Traits for circuits
// -----------------------------------------------------------------------------

namespace traits
{
	using nnf = std::integral_constant<circuit_t, NNF>;
	using dnnf = std::integral_constant<circuit_t, DNNF>;
	using dt = std::integral_constant<circuit_t, DT>;
	using edt = std::integral_constant<circuit_t, EDT>;
	using fbdd = std::integral_constant<circuit_t, FBDD>;
	using obdd = std::integral_constant<circuit_t, OBDD>;
	using sdd = std::integral_constant<circuit_t, SDD>;

	template<circuit_t C> struct to_circuit : public nnf {};
	template<> struct to_circuit<DNNF>: public dnnf {};
	template<> struct to_circuit<DT>: public dt {};
	template<> struct to_circuit<EDT>: public edt {};
	template<> struct to_circuit<FBDD>: public fbdd {};
	template<> struct to_circuit<OBDD>: public obdd {};
	template<> struct to_circuit<SDD>: public sdd {};
}

// -----------------------------------------------------------------------------
// Traits for circuits
// -----------------------------------------------------------------------------

namespace traits
{
	using co = std::integral_constant<query_t, CO>;
	using ct = std::integral_constant<query_t, CT>;
	using max = std::integral_constant<query_t, MAX>;
	using min = std::integral_constant<query_t, MIN>;

	template<query_t Q> struct to_query : public co {};
	template<> struct to_query<CT>: public ct {};
	template<> struct to_query<MAX>: public max {};
	template<> struct to_query<MIN>: public min {};

	struct additive : public true_type {};
	struct multiplicative : public false_type {};

	template<query_t Q> struct is_additive : public additive  {};
	template<> struct is_additive<CT>: public multiplicative {};
}

#endif
