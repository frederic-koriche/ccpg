// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// environment_full__.hpp
// -----------------------------------------------------------------------------

#ifndef ENVIRONMENT_FULL__
#define ENVIRONMENT_FULL__

#include "../fn/vec_ops__.hpp"

// -----------------------------------------------------------------------------
// Class Environment<feedback::full>
// Trials start from 1
// -----------------------------------------------------------------------------

template<language L>
class Environment<L, feedback::full>
{
	protected:              // Attributes
		const Circuit<L>& circuit__;
		const uword n_literals__;
		const uword n_objectives__;
		const uword n_trials__;
		const uword n_variables__;
		dmat objectives__;
		dvec target__;

	public:                 // Constructors & Destructor
		Environment(const Circuit<L>& circuit,
		            const uword n_objectives,
		            const uword n_trials) :
			circuit__(circuit),
			n_literals__(circuit.n_literals()),
			n_objectives__(n_objectives),
			n_trials__(n_trials),
			n_variables__(circuit.n_variables()),
			objectives__(circuit.n_literals(), n_objectives),
			target__(circuit__.n_literals())
		{
			assert((n_trials__ > 0) && (n_objectives__ > 0) && (n_objectives__ <= n_trials__));
			set_objectives();
			set_target();
		}

		~Environment()
		{
		}

	protected:              // Objective and target functions
		inline dvec get_objective(const uword trial) const
		{
			uword i = ((trial - 1) * n_objectives__) / n_trials__;
			return objectives__.col(i);
		}

		inline void set_objectives()
		{

			Sampler<language::dnnf> sample(circuit__);
			dvec assignment = sample();
			dvec objective = flip(assignment);
			mte generator;
			generator.seed(std::random_device()());
			for(uword i = 0; i < n_objectives__; ++i)
			{
				double n_swaps = (double) n_variables__ / 10.0;
				if(n_swaps < 1.0) n_swaps = 1.0;
				for(uword j = 0; j < n_swaps; ++j)
					{
						std::uniform_int_distribution<mte::result_type> dist(0, n_variables__ - 1);
						uword x = dist(generator);
						std::swap(assignment[2 * x], assignment[(2 * x) + 1]);
					}
				objectives__.col(i) = flip(assignment);
			}
		}

		inline void set_target()
		{
			dvec avg_objective = arma::sum(objectives__, 1);
			avg_objective /= n_objectives__;

			Optimizer<language::dnnf,query::min> optimizer(circuit__);
			target__ = optimizer(avg_objective);
		}


	public:
		// public env functions
		inline double loss(const dvec& prediction, const uword trial) const
		{
			return arma::dot(prediction, get_objective(trial));
		}

		inline dvec response(const uword trial) const
		{
			return get_objective(trial);
		}

		inline double regret(const dvec& prediction, const uword trial) const
		{
			dvec objective = response(trial);
			double pred_loss = arma::dot(objective, prediction);
			double targ_loss = arma::dot(objective, target__);
			return pred_loss - targ_loss;
		}

	public:
		friend ostream & operator <<(ostream & output, const Environment<L,feedback::full>& env)
		{
			cout << io::subsection("Environment objectives") << endl << env.objectives__ << endl;
			cout << io::subsection("Environment target") << endl << env.target__ << endl;
			return output;
		}
};

#endif
