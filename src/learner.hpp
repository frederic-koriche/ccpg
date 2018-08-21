// -----------------------------------------------------------------------------
//
// Online Combinatorial Optimization
// learner.hpp
//
// -----------------------------------------------------------------------------

#ifndef LEARNER
#define LEARNER

#include "environment.hpp"
#include "projector.hpp"

// -----------------------------------------------------------------------------
// Enum classes
// -----------------------------------------------------------------------------

// Forecasting strategy
enum class strategy
{
	exp, 	// Expanded Exponentially weighted average forecaster
	fpl,	// Follow the Perturbed Leader
	l2, 	// Online mirror descent forecaster with l2 regularizer (sgd)
	poly, 	// Online mirror descent forecaster with poly regularizer (linpoly)
	ure 	// Online mirror descent forecaster with ure regularizer (linexp)
};

// -----------------------------------------------------------------------------
// Classes
// -----------------------------------------------------------------------------

template<language L, feedback F, strategy S>class Learner;

#include "ml/learner_full_exp__.hpp"
#include "ml/learner_full_fpl__.hpp"
#include "ml/learner_full_l2__.hpp"
// #include "ml/learner_semibandit_exp__.hpp"
// #include "ml/learner_semibandit_l2__.hpp"
// #include "ml/learner_semibandit_ure__.hpp"
// #include "ml/learner_bandit_exp__.hpp"
//#include "ml/learner_bandit_lin__.hpp"

#endif // ifndef LEARNER
