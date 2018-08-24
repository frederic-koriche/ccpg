// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// learner_full_exp__.hpp
// -----------------------------------------------------------------------------

#ifndef LEARNER_FULL_EXP__
#define LEARNER_FULL_EXP__

// -----------------------------------------------------------------------------
// Class Learner<circuit_t C, EXPEXP, FULL>
// a.k.a Hedge for decisions compiled in nnf circuits
// -----------------------------------------------------------------------------

template<circuit_t C>
class Learner<C,EXPEXP,FULL>
{
	protected:
		// Attributes
		const Circuit<C>& circuit__;
		const Environment<C,FULL>& environment__;
		const uword n_literals__;
		const uword n_trials__;

	public:
		// Constructors & Destructor
		Learner(const Circuit<C>& circuit, const Environment<C,FULL>& environment, uword n_trials) :
			circuit__(circuit),
			environment__(environment),
			n_literals__(circuit.n_literals()),
			n_trials__(n_trials)
		{
		}

		~Learner()
		{
		}

	protected:
		inline void set_hyperparameters(double& partition)
		{
			Counter<C> count(circuit__);
			partition = count();
		}

		inline void update_distribution(dvec& distribution, const dvec& cumloss, const double& eta)
		{
			for(uword x = 0; x < n_literals__; x++)
				distribution[x] = exp(-eta * cumloss[x]);
		}

		inline void update_hyperparameters(double& eta, const double& partition, const uword& trial)
		{
			double logN = log(partition);
			eta = sqrt(logN / (2.0 * (double) trial));
		}

		inline void update_loss(dvec& cumloss, const dvec& objective)
		{
			cumloss += objective;
		}

	public:
		// public learning functions
		inline void learn()
		{
			cout << io::subsection("Initializing learner") << endl;
			double cum_regret = 0;
			double eta = 0;
			double partition = 0;
			set_hyperparameters(partition);
			cout << io::info("partition") << partition << endl;

			Sampler<C> sample(circuit__);
			dvec distribution(n_literals__, arma::fill::ones);
			dvec cumloss(n_literals__, arma::fill::zeros);

			cout << io::subsection("Learning") << endl;
			for(uword trial = 1; trial <= n_trials__; trial++)
			{
				// Update hyperparameters
				update_hyperparameters(eta, partition, trial);
				cout << io::info("Updating hyperparameters") << trial << " [eta]: " << eta << endl;

				// Sample a model
				dvec prediction = sample(distribution);
				//cout << io::info("Prediction") << trial << endl << prediction << endl;

				// Get response
				dvec objective = environment__.response(trial);
				//cout << io::info("Objective") << trial << endl << objective << endl;
				cout << io::info("Loss") << trial << " [loss]: " << arma::dot(prediction, objective) << endl;
				double regret = environment__.regret(prediction, trial);
				cout << io::info("Regret") << trial << " [reg]: " << regret << endl;
				cum_regret += regret;

				// Update cumulative loss
				update_loss(cumloss, objective);

				// Update distribution
				update_distribution(distribution, cumloss, eta);
				//cout << io::info("Updating distribution") << trial << endl << distribution << endl;
			}
			cum_regret /= (double) n_trials__;
			cout << io::info("Cumulative regret") << cum_regret << endl;
		}
};

#endif
