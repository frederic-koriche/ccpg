// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// learner_full_l2__.hpp
// -----------------------------------------------------------------------------

#ifndef LEARNER_FULL_L2__HPP
#define LEARNER_FULL_L2__HPP

// -----------------------------------------------------------------------------
// Class Learner<circuit_t C, SGD, FULL>
// a.k.a SGD for decisions compiled in NNF circuits
// -----------------------------------------------------------------------------

template<circuit_t C>
class Learner<C, SGD, FULL>
{
	protected:
		// Attributes
		const Circuit<C>& circuit__;
		const Environment<C, FULL>& environment__;
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

		inline void update_weights(dvec& distribution, const dvec& loss, const double& eta)
		{
			distribution -= eta * loss;
		}

	public:
		// public learning functions
		inline void learn()
		{
			cout << io::subsection("Initializing learner") << endl;

			Regularizer<L2> reg;
			Projector<DNNF,PCG,L2> project(circuit__,reg,100);
			dvec weights(n_literals__, arma::fill::zeros);
			double cum_regret = 0;
			double epsilon = 0;
			double eta = 0;
			double gamma = 0;

			cout << io::subsection("Learning") << endl;
			for(uword trial = 1; trial <= n_trials__; trial++)
			{
				// Update hyperparameters
				eta = 1.0 / sqrt((double) trial);
				gamma = (double) n_literals__ / 2.0;
				epsilon = gamma / ((double) trial);
				cout << io::info("hyperparameters") << "[eta] " << eta << " [gamma] " << gamma << " [epsilon] " << epsilon << endl;

				// Project and decompose
				dvec prediction = project(weights, epsilon);

				// Get response
				dvec feedback = environment__.response(trial);
				//cout << io::info("Objective") << trial << endl << feedback << endl;
				cout << io::info("Loss") << trial << " [loss]: " << arma::dot(prediction, feedback) << endl;
				double regret = environment__.regret(prediction, trial);
				cum_regret += regret;
				cout << io::info("Regret") << trial << " [regret]: " << cum_regret / (double) trial << endl;

				// Update weights
				weights -= eta * feedback;
			}
			cum_regret /= (double) n_trials__;
			cout << io::info("Cumulative regret") << cum_regret << endl;
		}
};

#endif
