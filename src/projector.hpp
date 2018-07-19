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
	re,   					// (epsilon) relative entropy
	ure     				// (epsilon) unormalized relative entropy
};

// -----------------------------------------------------------------------------
// Classes
// -----------------------------------------------------------------------------

template<language L,regularizer R,policy P> class Projector__;
template<language L,regularizer R,policy P> class Projector;

//#include "ml/projector_cg__.hpp"
#include "ml/projector_pcg__.hpp"

#endif
