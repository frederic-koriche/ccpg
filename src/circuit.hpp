// -----------------------------------------------------------------------------
//
// Online Combinatorial Optimization
// circuit.hpp
//
// -----------------------------------------------------------------------------

#ifndef CIRCUIT
#define CIRCUIT

#include "io.hpp"

// -----------------------------------------------------------------------------
// Enum classes
// -----------------------------------------------------------------------------

// Circuit language
enum class language
{
	nnf,                    // Negation Normal Form Formula
	dnnf,                   // deterministic Decomposable Negation Normal Form formula
	dt,                     // Decision Tree
	edt,            // Extended Decision Tree
	fbdd,   // Free Binary Decision Diagram
	obdd,           // Ordered Binary Decision Diagram
	sdd                     // Sentential Decision Diagram
};

// Circuit query
enum class query
{
	co,
	ct,
	min,
	max
};

// -----------------------------------------------------------------------------
// Traits (1xx)
// -----------------------------------------------------------------------------

namespace traits
{
using nnf = std::integral_constant<language, language::nnf>;
using dnnf = std::integral_constant<language, language::dnnf>;
using dt = std::integral_constant<language, language::dt>;
using edt = std::integral_constant<language, language::edt>;
using fbdd = std::integral_constant<language, language::fbdd>;
using obdd = std::integral_constant<language, language::obdd>;
using sdd = std::integral_constant<language, language::sdd>;

template<language L> struct to_language : public nnf {};
template<> struct to_language<language::dnnf>: public dnnf {};
template<> struct to_language<language::dt>: public dt {};
template<> struct to_language<language::edt>: public edt {};
template<> struct to_language<language::fbdd>: public fbdd {};
template<> struct to_language<language::obdd>: public obdd {};
template<> struct to_language<language::sdd>: public sdd {};

using co = std::integral_constant<query, query::co>;
using ct = std::integral_constant<query, query::ct>;
using max = std::integral_constant<query, query::min>;
using min = std::integral_constant<query, query::max>;

template<query Q> struct to_query : public co {};
template<> struct to_query<query::ct>: public ct {};
template<> struct to_query<query::max>: public max {};
template<> struct to_query<query::min>: public min {};

struct additive : public true_type {};
struct multiplicative : public false_type {};

template<query Q> struct is_additive : public additive  {};
template<> struct is_additive<query::ct>: public multiplicative {};
}

// -----------------------------------------------------------------------------
// Classes
// -----------------------------------------------------------------------------

template<language L> class Circuit;
template<language L, unsigned int T> class Marginalizer;
template<language L> class Counter;
template<language L> class Counter__;
template<language L, query Q> class Engine__;
template<language L> class Node;
template<language L, query Q> class Optimizer;
template<language L, query Q> class Optimizer__;
template<language L> class Sampler;
template<language L> class Sampler__;
template<language L> ostream & operator<<(ostream&, const Circuit<L>&);
template<query Q> struct conv_to;

template<language L> using UnivariateMarginalizer = Marginalizer<L,1>;
template<language L> using BivariateMarginalizer = Marginalizer<L,2>;

using NnfNode = Node<language::nnf>;

using Dnnf = Circuit<language::dnnf>;
using DnnfCounter = Counter<language::dnnf>;
using DnnfSampler = Sampler<language::dnnf>;
template<query Q> using DnnfOptimizer = Optimizer<language::dnnf, Q>;
using DnnfMaximizer = Optimizer<language::dnnf, query::max>;
using DnnfMinimizer = Optimizer<language::dnnf, query::min>;

using Sdd = Circuit<language::sdd>;
using SddCounter = Counter<language::sdd>;
using SddSampler = Sampler<language::dnnf>;

#include "ai/dnnf_circuit__.hpp"
#include "ai/dnnf_counter__.hpp"
#include "ai/dnnf_sampler__.hpp"
#include "ai/dnnf_marginalizer__.hpp"
#include "ai/dnnf_optimizer__.hpp"

// #include "ai/dnnf_estimator_bivariate__.hpp"
// #include "ai/sdd_circuit__.hpp"
// #include "ai/sdd_counter_.hpp"

#endif
