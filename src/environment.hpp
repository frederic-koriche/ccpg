// -----------------------------------------------------------------------------
//
// Online Combinatorial Optimization
// environment.hpp
//
// -----------------------------------------------------------------------------

#ifndef ENVIRONMENT
#define ENVIRONMENT

#include "circuit.hpp"

// -----------------------------------------------------------------------------
// Enum classes
// -----------------------------------------------------------------------------

// Environment behavior
enum class behavior
{
	stochastic,
	adversarial
};

// Feedback type
enum class feedback
{
	full,
	semibandit,
	bandit
};

template<language L, feedback F> class Environment;

#include "ml/environment_full__.hpp"
// #include "ml/environment_semibandit__.hpp"
// #include "ml/environment_bandit__.hpp"

#endif
