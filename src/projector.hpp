// -----------------------------------------------------------------------------
//
// Online Combinatorial Optimization
// projector.hpp
//
// -----------------------------------------------------------------------------

#ifndef PROJECTOR
#define PROJECTOR

#include "circuit.hpp"

// -----------------------------------------------------------------------------
// Enum classes
// -----------------------------------------------------------------------------

// Convex optimization policy
enum class policy
{
	cg,                     // Conditional gradient
	pcg                     // Pairwise conditional gradient
};

// Regularization function
enum class regularizer
{
	l2,  					// L2 norm
	re,   					// (shift+) relative entropy
	ure     				// (shift+) unormalized relative entropy
};

// -----------------------------------------------------------------------------
// Traits
// -----------------------------------------------------------------------------

namespace traits
{
using l2 = std::integral_constant<regularizer, regularizer::l2>;
using re = std::integral_constant<regularizer, regularizer::re>;
using ure = std::integral_constant<regularizer, regularizer::ure>;

template<regularizer L> struct to_regularizer : public l2 {};
template<> struct to_regularizer<regularizer::re>: public re {};
template<> struct to_regularizer<regularizer::ure>: public ure {};
}

// -----------------------------------------------------------------------------
// Classes
// -----------------------------------------------------------------------------

template<language L,regularizer R,policy P> class Projector;

//#include "ml/projector_cg__.hpp"
#include "ml/projector_pcg__.hpp"

#endif
