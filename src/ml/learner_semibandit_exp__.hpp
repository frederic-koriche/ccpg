// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// learner_semibandit_exp__.hpp
// -----------------------------------------------------------------------------

#ifndef ML_LEARNER_SEMIBANDIT_EXP__HPP
#define ML_LEARNER_SEMIBANDIT_EXP__HPP

// -----------------------------------------------------------------------------
// Learner<L, feedback::semibandit, strategy::exp>
// a.k.a Exp2 for objets compiled in dnnf circuits
// -----------------------------------------------------------------------------

template<language L>
class Learner<L, feedback::semibandit, strategy::exp>
{
	protected:
		// Attributes
		const Circuit<L>& circuit__;
		const Environment<L, feedback::semibandit>& environment__;
		const uword n_trials__;
		const uword n_variables__;

	public:
		// Constructors & Destructor
		Learner(const Circuit<L>& circuit, const Environment<L, feedback::semibandit>& environment, uword n_trials) :
			circuit__(circuit),
			environment__(environment),
			n_trials__(n_trials),
			n_variables__(circuit.n_variables())
		{
		}

		~Learner()
		{
		}

	protected:
		inline void set_hyperparameters(double& partition)
		{
			Counter<L> count(circuit__);
			partition = count();
		}

		inline void update_distribution(dvec& distribution, const dvec& cumloss, const double& eta)
		{
			for(uword x = 0; x < n_variables__; x++)
				distribution[x] = exp(-eta * cumloss[x]);
		}

		inline void update_hyperparameters(double& eta, double& gamma, const double& partition, const uword& trial)
		{
			double logN = log(partition);
			gamma = sqrt(1.0 / (2.0 * (double) trial));
			eta = sqrt((1.0 - gamma) * logN / (2.0 * (double) trial));
		}

		inline void update_loss(dvec& cumloss, const dvec& cost, const dvec& marginals, const dvec& model)
		{
			dvec loss(n_variables__, arma::fill::zeros);
			for(uword x = 0; x < n_variables__; x++)
				if(marginals[x] > 0.0)
					loss[x] = model[x] * (cost[x] / marginals[x]);
			cumloss += loss;
		}

	public:
		// public learning functions
		inline void learn()
		{
			cout << io::subsection("Initializing learner") << endl;
			double eta = 0;
			double gamma = 0;
			double partition = 0;
			set_hyperparameters(partition);
			cout << io::info("partition") << partition << endl;

			UnivariateEstimator<L> marginalize(circuit__);
			Sampler<L> sample(circuit__);
			dvec uniform(n_variables__, arma::fill::ones);
			dvec distribution(n_variables__, arma::fill::ones);
			dvec cumloss(n_variables__, arma::fill::zeros);

			cout << io::subsection("Learning") << endl;
			for(uword trial = 1; trial <= n_trials__; trial++)
			{
				// Update hyperparameters
				update_hyperparameters(eta, gamma, partition, trial);
				cout << io::info("Updating hyperparameters") << trial << " [eta]: " << eta << " [gamma]: " << gamma << endl;

				// Sample a model
				dvec model = sample(conv_to<Ct>::from(uniform), conv_to<Ct>::from(distribution), gamma);
				//dvec model = sample(conv_to<Ct>::from(distribution));
				//cout << io::info("Sampling model") << trial << endl << model << endl;

				// Get response
				//cout << io::info("Objective") << endl << environment__.objective() << endl;
				dvec cost = environment__.response(model);
				//cout << io::info("Receiving cost vector") << trial << endl << cost << endl;
				cout << io::info("Cost") << trial << " [cost]: " << arma::dot(cost, model) << endl;

				// Update cumulative loss
				dvec marginals = marginalize(conv_to<Ct>::from(distribution));
				update_loss(cumloss, cost, marginals, model);
				//cout << io::info("Updating loss function") << trial << endl << cumloss << endl;

				// Update distribution
				update_distribution(distribution, cumloss, eta);
				//cout << io::info("Updating distribution") << trial << endl << distribution << endl;
			}
		}
};

#endif
