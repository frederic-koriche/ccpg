// -----------------------------------------------------------------------------
//
// Online Combinatorial Optimization
// learner.hpp
//
// -----------------------------------------------------------------------------

#ifndef LEARNER_HPP
#define LEARNER_HPP

#include "reasoner.hpp"

template<circuit_t C, environment_t E> class Environment;
template<circuit_t C, algorithm_t A, environment_t E> class Learner;
template<circuit_t C, algorithm_t A, distance_t D> class Projector;
template<distance_t D> class Regularizer;

#include "ml/environment_full__.hpp"
// #include "ml/environment_semibandit__.hpp"
// #include "ml/environment_bandit__.hpp"
#include "ml/regularizer_l2__.hpp"
#include "ml/regularizer_ure__.hpp"
#include "ml/projector_pcg__.hpp"
#include "ml/learner_expexp_full__.hpp"
#include "ml/learner_fpl_full__.hpp"
#include "ml/learner_sgd_full__.hpp"
// #include "ml/learner_full_l2__.hpp"
// #include "ml/learner_semibandit_exp__.hpp"
// #include "ml/learner_semibandit_l2__.hpp"
// #include "ml/learner_semibandit_ure__.hpp"
// #include "ml/learner_bandit_exp__.hpp"
//#include "ml/learner_bandit_lin__.hpp"

#endif
