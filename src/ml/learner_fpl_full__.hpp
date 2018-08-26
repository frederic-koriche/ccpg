// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// learner_full_fpl__.hpp
// -----------------------------------------------------------------------------

#ifndef LEARNER_FULL_FPL__HPP
#define LEARNER_FULL_FPL__HPP

// -----------------------------------------------------------------------------
// Class Learner<circuit_t C, FPL, FULL>
// a.k.a FPL for decisions compiled in nnf circuits
// -----------------------------------------------------------------------------

template<circuit_t C>
class Learner<C, FPL, FULL>
{
	protected:
		// Attributes
		const Circuit<C>& circuit__;
		const Environment<C,FULL>& environment__;
		const uword n_literals__;
		const uword n_trials__;
		mte* generator__;

	public:
		// Constructors & Destructor
		Learner(const Circuit<C>& circuit, const Environment<C,FULL>& environment, uword n_trials) :
			circuit__(circuit),
			environment__(environment),
			n_literals__(circuit.n_literals()),
			n_trials__(n_trials),
			generator__(nullptr)
		{
		}

		~Learner()
		{
		}

	protected:
		inline void update_perturbation(dvec& perturbation)
		{
			for(uword x = 0; x < n_literals__; x++)
			{
				std::bernoulli_distribution dis(0.5);
				if(dis(*generator__))
					perturbation(x) += 0.5;
				else
					perturbation(x) -= 0.5;
			}
		}

	public:
		// public learning functions
		inline void learn()
		{
			cout << io::subsection("Initializing learner") << endl;
			Minimizer<C> minimize(circuit__);

			std::random_device rd;
			mte gen(rd());
			generator__ = &gen;
			dvec perturbation(n_literals__, arma::fill::zeros);

			dvec cum_loss(n_literals__, arma::fill::zeros);
			dvec per_loss(n_literals__, arma::fill::zeros);
			double cum_regret = 0;

			cout << io::subsection("Learning") << endl;
			for(uword trial = 1; trial <= n_trials__; trial++)
			{
				// Upodate perturbation
				update_perturbation(perturbation);
				per_loss = cum_loss + perturbation;

				// Follow the Perturbed Leader
				dvec prediction = minimize(per_loss);

				// Get response
				dvec objective = environment__.response(trial);
				//cout << io::info("Objective") << trial << endl << objective << endl;
				cout << io::info("Loss") << trial << " [loss]: " << arma::dot(prediction, objective) << endl;
				double regret = environment__.regret(prediction, trial);
				cout << io::info("Regret") << trial << " [reg]: " << regret << endl;
				cum_regret += regret;

				// Update cumulative loss
				cum_loss += objective;
			}
			cum_regret /= (double) n_trials__;
			cout << io::info("Cumulative regret") << cum_regret << endl;
		}
};

#endif
