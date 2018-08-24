// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// dnnf_marginalizer__.hpp
// -----------------------------------------------------------------------------

#ifndef DNNF_MARGINALIZER__
#define DNNF_MARGINALIZER__

#include "dnnf_counter__.hpp"

// -----------------------------------------------------------------------------
// Class Marginalizer<DNNF,1>
// Computes the univariate distribution (literal probabilities) for the dDNNF
// Literals weights: even index (positive literal) odd index (negative literal)
// -----------------------------------------------------------------------------

template<>
class Marginalizer<DNNF,1> final : public Counter__<DNNF>
{
	public:                 // Traits
		using base_type = Counter__<DNNF>;

	protected:              // Attributes
		using base_type::circuit__;
		using base_type::n_literals__;
		using base_type::n_nodes__;
		using base_type::n_variables__;

	public:                 // Constructors & Destructor
		Marginalizer(const Circuit<DNNF>& circuit) :
			base_type(circuit)
		{
		}

		~Marginalizer()
		{
		}

	protected:              // Autocorrelation functions
		inline void threaded_marginalize(dvec& marginals, const dvec& dis, const double& partition, const uword x_min, const uword x_max)
		{
			dvec distribution(dis);
			dvec weights(n_nodes__, arma::fill::zeros);

			for(uword x = x_min; x < x_max; ++x)
			{
				double wx = distribution[(2 * x) + 1];
				distribution[(2 * x) + 1] = 0;
				push_weights(weights, distribution);
				double xpartition = weights[n_nodes__ - 1];
				marginals[2 * x] = xpartition / partition;
				marginals[(2 * x) + 1] =  1.0 - marginals[2 * x];
				distribution[(2 * x) + 1] = wx;
			}
		}

		inline dvec marginalize(const dvec& distribution)
		{
			dvec weights(n_nodes__, arma::fill::zeros);
			dvec marginals(n_literals__, arma::fill::zeros);
			push_weights(weights, distribution);
			double partition = weights[n_nodes__ - 1];

			uword n_threads = std::thread::hardware_concurrency();
			if(n_threads > n_variables__) n_threads = n_variables__;
			cout << io::info("Number of threads used") << n_threads << endl;

			std::vector<std::future<void> > threads;
			uword chunk_size = n_variables__ / n_threads;
			uword x_min = 0;
			uword x_max = 0;
			for(uword t = 0; t < n_threads; ++t)
			{
				x_min = x_max;
				x_max = x_min + chunk_size;
				if(t == n_threads - 1) x_max = n_variables__;
				threads.push_back(std::async(std::launch::async,
				                             &Marginalizer::threaded_marginalize,
				                             this,
				                             std::ref(marginals),
				                             std::ref(distribution),
				                             partition,
				                             x_min,
				                             x_max));
			}

			for(auto & t: threads)
				t.get();

			return marginals;
		}

	public:                 // Marginalization operators
		inline dvec operator()()
		{
			dvec distribution(circuit__.n_literals(), arma::fill::ones);
			return marginalize(distribution);
		}

		inline dvec operator()(const dvec& distribution)
		{
			assert(distribution.n_elem == n_literals__);
			return marginalize(distribution);
		}
};

#endif
