// -----------------------------------------------------------------------------
//
// Online Combinatorial Optimization
// enums.hpp
//
// -----------------------------------------------------------------------------

#ifndef ENUMS_HPP
#define ENUMS_HPP

#include "types.hpp"

// -----------------------------------------------------------------------------
// algorithms
// -----------------------------------------------------------------------------

enum algorithm_t
{
	CG,             // Conditional gradient
	EXPEXP,         // Expanded Exponentially weighted average forecaster
	FPL,            // Follow the Perturbed Leader
	PCG,            // Pairwise conditional gradient
	LINPOLY,        // Online mirror descent forecaster with poly regularizer (linpoly)
	LINEXP,         // Online mirror descent forecaster with ure regularizer (linexp)
	SGD             // Online mirror descent forecaster with l2 regularizer (sgd)
};

// -----------------------------------------------------------------------------
// Distance generating functions
// -----------------------------------------------------------------------------

enum distance_t
{
	L2,     // L2 norm function
	RE,     // Relative entropy (KL divergence)
	URE     // Unormalized relative entropy
};

// -----------------------------------------------------------------------------
// Environment feedbacks
// -----------------------------------------------------------------------------

enum environment_t
{
	FULL,
	SEMIBANDIT,
	BANDIT
};

// -----------------------------------------------------------------------------
// Circuit classes
// -----------------------------------------------------------------------------

enum circuit_t
{
	NNF,    // Negation Normal Form
	DNNF,   // (deterministic) Decomposable Negation Normal Form
	DT,     // Decision Tree
	EDT,    // Extended Decision Tree
	FBDD,   // Free Binary Decision Diagram
	OBDD,   // Ordered Binary Decision Diagram
	SDD     // Sentential Decision Diagram
};

// -----------------------------------------------------------------------------
// Queries
// -----------------------------------------------------------------------------

enum query_t
{
	CO,     // Coherence testing
	CT,     // Model counting
	MAX,    // Maximization
	MIN     // Minimization
};

#endif
