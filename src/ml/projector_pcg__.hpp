// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// projector_pcg__.hpp
// -----------------------------------------------------------------------------

#ifndef PROJECTOR_PCG__HPP
#define PROJECTOR_PCG__HPP

// -----------------------------------------------------------------------------
// Projector__<circuit_t C, PCG, distance_t D>
// PCG: Pairwise Conditional gradient
// PCG-based Bregman projector subject to NNF constraint
// -----------------------------------------------------------------------------

template<circuit_t C, distance_t D>
class Projector<C,PCG,D>
{
	protected:
		// Attributes
		const Circuit<C>& circuit__;
		const Regularizer<D>& regularizer__;
		const uword max_trials__;
		const uword n_line_steps__;
		const uword n_literals__;

	public:
		// Constructors & Destructor
		Projector(const Circuit<C>& circuit,
		          const Regularizer<D>& regularizer,
		          uword max_trials=std::numeric_limits<uword>::infinity(),
		          uword n_line_steps=10) :
			circuit__(circuit),
			regularizer__(regularizer),
			max_trials__(max_trials),
			n_line_steps__(n_line_steps),
			n_literals__(circuit.n_literals())
		{
		}

		~Projector()
		{
		}

	protected:
		// Away step in pairwise conditional gradient
		inline uword away_step(const dvec& grad, const dmat& assignments, uword trial)
		{
			uword best_index = 0;
			double best_value = -std::numeric_limits<double>::infinity();
			for(uword t = 0; t < trial; t++)
			{
				double value = arma::dot(assignments.col(t), grad);
				if(value > best_value)
				{
					best_index = t;
					best_value = value;
				}
			}
			return best_index;
		}

		// Convert accuracy to trials
		inline uword estimate(const double& accuracy, const double& alpha, const double& beta)
		{
			double l = (double) n_literals__;
			double a = 8.0 * l * beta / alpha;
			double b = log( beta * l) - log(2.0 * accuracy);
			uword n_trials = (uword) a * b;
			if(n_trials == 0)
			 	return 1;
			if(n_trials > max_trials__)
				return max_trials__;
			return n_trials;
		}

		// Find stepsize via line search
		inline double line_search(const dvec& point, const dvec& fw_assignment, const dvec& away_assignment, const double& away_prob)
		{
			double left = 0.0;
			double right = away_prob;
			dvec diff = fw_assignment - away_assignment;
			uword i = 0;
			while((left < right) && i < n_line_steps__)
			{
				double middle = (right + left)/2.0;
				dvec line = point + (middle * diff);
				if(arma::all(line >= 0.0))
					left = middle;
				else
					right = middle;
				i++;
			}
			return left;
		}

		// Bregman projection via PCG
		void project(dvec& distribution, dmat& assignments, const dvec& weights, const uword n_trials)
		{
			Sampler<C> sample(circuit__);
			dvec point = sample();
			//cout << io::info("initial point") << endl << point << endl;
			assignments.col(0) = point;
			distribution[0] = 1.0;

			Minimizer<C> minimizer(circuit__);

			//cout << io::info("Projection") << endl;
			for(uword trial = 1; trial < n_trials; trial++)
			{
				dvec grad = regularizer__.gradient(point, weights);
				//cout << io::info("gradient") << trial << endl << grad << endl;

				dvec fw_assignment = minimizer(grad);
				//cout << io::info("fw assignment") << trial << endl << fw_assignment << endl;

				uword index = away_step(grad, assignments, trial);
				double away_prob = distribution[index];
				dvec away_assignment = assignments.col(index);
				//cout << io::info("away assignment") << trial << endl << away_assignment << endl;

				double gamma = line_search(point, fw_assignment, away_assignment, away_prob);
				//cout << io::info("stepsize") << gamma << endl;
				distribution[index] -= gamma;
				distribution[trial] = gamma;
				assignments.col(trial) = fw_assignment;
				//cout << io::info("distribution") << trial << endl << distribution << endl;

				point += gamma * (fw_assignment - away_assignment);
				//cout << io::info("point") << trial << endl << point << endl;
			}
		}

		// Sample from Bregman decomposition
		inline dvec sample(const dvec& distribution, const dmat& assignments)
		{
			std::random_device rd;
			mte generator(rd());
			std::discrete_distribution<int> discrete(distribution.begin(),distribution.end());
			uword index = (uword) discrete(generator);
			return assignments.col(index);
		}

	public:
		inline dvec operator()(const dvec& weights, const double& accuracy)
		{
			uword n_trials = estimate(accuracy, regularizer__.alpha(), regularizer__.beta());
			cout << io::info("estimate trials") << n_trials << endl;

			dvec distribution(n_trials, arma::fill::zeros);
			dmat assignments(n_literals__, n_trials, arma::fill::zeros);

			project(distribution, assignments, weights, n_trials);
			return sample(distribution, assignments);
		}
};

#endif
