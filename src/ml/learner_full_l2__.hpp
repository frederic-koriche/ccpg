// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// learner_full_l2__.hpp
// -----------------------------------------------------------------------------

#ifndef LEARNER_FULL_L2__
#define LEARNER_FULL_L2__

// -----------------------------------------------------------------------------
// Class Learner<L, feedback::full, strategy::l2>
// a.k.a SGD for decisions compiled in nnf circuits
// -----------------------------------------------------------------------------

template<language L>
class Learner<L, feedback::full, strategy::l2>
{
	protected:
		// Attributes
		const Circuit<L>& circuit__;
		const Environment<L, feedback::full>& environment__;
		const uword n_literals__;
		const uword n_trials__;

	public:
		// Constructors & Destructor
		Learner(const Circuit<L>& circuit, const Environment<L, feedback::full>& environment, uword n_trials) :
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
		inline void update_distribution(dvec& distribution, const dvec& cum_loss, const double& eta)
		{
			for(uword x = 0; x < n_literals__; x++)
				distribution[x] = exp(-eta * cum_loss[x]);
		}

		inline void update_hyperparameters(double& epsilon, double& eta, double& gamma, const uword& trial)
		{
			eta = 1.0 / sqrt((double) trial);
			gamma = (double) n_literals__ / 2.0;
			epsilon = gamma / ((double) trial);
		}

	public:
		// public learning functions
		inline void learn()
		{
			cout << io::subsection("Initializing learner") << endl;

			Projector<L,regularizer::l2,policy::pcg> project(circuit__,0.0,10);
			dvec distribution(n_literals__, arma::fill::ones);
			dvec cum_loss(n_literals__, arma::fill::zeros);
			double cum_regret = 0;
			double epsilon = 0;
			double eta = 0;
			double gamma = 0;

			cout << io::subsection("Learning") << endl;
			for(uword trial = 1; trial <= n_trials__; trial++)
			{
				// Update hyperparameters
				update_hyperparameters(epsilon, eta, gamma, trial);
				cout << io::info("hyperparameters") << "[epsilon] " << epsilon << endl;

				// Project and decompose
				dvec prediction = project(distribution, epsilon);

				// Get response
				dvec objective = environment__.response(trial);
				//cout << io::info("Objective") << trial << endl << objective << endl;
				cout << io::info("Loss") << trial << " [loss]: " << arma::dot(prediction, objective) << endl;
				double regret = environment__.regret(prediction, trial);
				cout << io::info("Regret") << trial << " [reg]: " << regret << endl;
				cum_regret += regret;

				// Update cumulative loss
				cum_loss += objective;

				// Update distribution
				update_distribution(distribution, cum_loss, eta);
				//cout << io::info("Updating distribution") << trial << endl << distribution << endl;
			}
			cum_regret /= (double) n_trials__;
			cout << io::info("Cumulative regret") << cum_regret << endl;
		}
};

#endif