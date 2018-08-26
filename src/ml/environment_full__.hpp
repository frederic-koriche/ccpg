// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// environment_full__.hpp
// -----------------------------------------------------------------------------

#ifndef ENVIRONMENT_FULL__HPP
#define ENVIRONMENT_FULL__HPP

// -----------------------------------------------------------------------------
// Class Environment<FULL>
// Trials start from 1
// -----------------------------------------------------------------------------

template<circuit_t C>
class Environment<C,FULL>
{
	protected:              // Attributes
		const Circuit<C>& circuit__;
		const uword n_literals__;
		const uword n_objectives__;
		const uword n_trials__;
		const uword n_variables__;
		dmat objectives__;
		dvec target__;

	public:                 // Constructors & Destructor
		Environment(const Circuit<C>& circuit, const uword n_objectives, const uword n_trials) :
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
			Sampler<C> sample(circuit__);
			dmat assignments = sample(n_objectives__);
			dmat ones(n_literals__,n_objectives__,arma::fill::ones);
			objectives__ = ones - assignments;
		}

		inline void set_target()
		{
			dvec avg_objective = arma::sum(objectives__, 1);
			avg_objective /= n_objectives__;
			Minimizer<C> minimizer(circuit__);
			target__ = minimizer(avg_objective);
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
		friend ostream & operator <<(ostream & output, const Environment<C,FULL>& env)
		{
			cout << io::subsection("Environment objectives") << endl << env.objectives__ << endl;
			cout << io::subsection("Environment target") << endl << env.target__ << endl;
			return output;
		}
};

#endif
