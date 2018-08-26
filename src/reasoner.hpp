// -----------------------------------------------------------------------------
//
// Online Combinatorial Optimization
// reasoner.hpp
//
// -----------------------------------------------------------------------------

#ifndef REASONER_HPP
#define REASONER_HPP

#include "io.hpp"

template<circuit_t C> class Circuit;
template<circuit_t C> class Counter;
template<circuit_t C> class Counter__;
template<circuit_t C, query_t Q> class Engine__;
template<circuit_t C, query_t Q> class Optimizer;
template<circuit_t C, query_t Q> class Optimizer__;
template<circuit_t C, unsigned int I> class Marginalizer;
template<circuit_t C> class Sampler;
template<circuit_t C> class Sampler__;
template<query_t Q> struct conv_to;

template<circuit_t C> using UnivariateMarginalizer = Marginalizer<C,1>;
template<circuit_t C> using BivariateMarginalizer = Marginalizer<C,2>;
template<circuit_t C> using Maximizer = Optimizer<C, MAX>;
template<circuit_t C> using Minimizer = Optimizer<C, MIN>;

#include "ai/dnnf_circuit__.hpp"
#include "ai/dnnf_counter__.hpp"
#include "ai/dnnf_sampler__.hpp"
#include "ai/dnnf_marginalizer__.hpp"
#include "ai/dnnf_optimizer__.hpp"
// #include "ai/dnnf_estimator_bivariate__.hpp"
// #include "ai/sdd_circuit__.hpp"
// #include "ai/sdd_counter_.hpp"

#endif
