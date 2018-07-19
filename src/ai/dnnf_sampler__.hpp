// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// dnnf_sampler__.hpp
// -----------------------------------------------------------------------------

#ifndef DNNF_SAMPLER__
#define DNNF_SAMPLER__

#include "dnnf_counter__.hpp"

// -----------------------------------------------------------------------------
// Abstract class Sampler__<language::dnnf>
// Weighted assignment sampler for DNNF
// Literals weights: even index (positive literal) odd index (negative literal)
// -----------------------------------------------------------------------------

template<>
class Sampler__<language::dnnf>: public Counter__<language::dnnf>
{
	public:
		// Traits
		using base_type = Counter__<language::dnnf>;
		using Children = arma::SpSubview<uword>;

	protected:
		// Attributes
		using base_type::circuit__;
		using base_type::n_literals__;
		using base_type::n_nodes__;
		mte* generator__;
		dvec distribution__;
		sp_dmat edge_weights__;
		dvec node_weights__;

	public:
		// Constructors & Destructor
		Sampler__(const Dnnf& circuit) :
			base_type(circuit),
			generator__(nullptr),
			distribution__(circuit.n_literals()),
			edge_weights__(circuit.n_nodes(), circuit.n_nodes()),
			node_weights__(circuit.n_nodes())
		{
		}

		~Sampler__()
		{
		}

	protected:
		// Protected sampling operations
		uword sample_child_variable(const uword parent, const Children& children)
		{
			double partition = node_weights__[parent];
			double prob = 0;
			auto p = children.begin();
			uword child = p.col();
			bool is_chosen = false;
			while(!is_chosen and p != children.end())
			{
				child = p.col();
				prob += (edge_weights__(parent, child) / partition);
				std::bernoulli_distribution dis(prob);
				is_chosen = dis(*generator__);
				++p;
			}
			return child;
		}

		void sample_free_variables(dvec& assignment, const uword parent, const uword child)
		{
			uvec vars = circuit__.edge_label(parent, child);
			for(uword i = 0; i < vars.size(); ++i)
			{
				uword x = vars[i];
				double pos_weight = distribution__[2 * x];
				double neg_weight = distribution__[(2 * x) + 1];
				double prob = pos_weight / (neg_weight + pos_weight);
				std::bernoulli_distribution dis(prob);
				assignment[2 * x] = (double)dis(*generator__);
				assignment[(2 * x) + 1] = 1.0 - assignment[2 * x];
			}
		}

		void sample_assignment(dvec& assignment, const uword index)
		{
			char type = circuit__.node_label(index).type;

			//assert(type != 'f');

			if(type == 'f')
				return;

			if(type == 't')
				return;

			if(type == 'l')
			{
				uword x = circuit__.node_label(index).vars[0];
				assignment[2 * x] = (double)circuit__.node_label(index).sgn;
				assignment[(2 * x) + 1] = 1.0 - assignment[2 * x];
				return;
			}

			const arma::SpSubview<uword> children = circuit__.out_edges(index);

			if(type == 'a')
			{
				for(auto p = children.begin(); p != children.end(); ++p)
					sample_assignment(assignment, p.col());
				return;
			}

			uword child = sample_child_variable(index, children);
			sample_free_variables(assignment, index, child);
			sample_assignment(assignment, child);
		}

		void threaded_multi_sample(dmat& assignments, const uword c_min, const uword c_max)
		{
			for(uword c = c_min; c < c_max; ++c)
			{
				dvec assignment(n_literals__, arma::fill::zeros);
				sample_assignment(assignment, n_nodes__ - 1);
				assignments.col(c) = assignment;
			}
		}

		void multi_sample(dmat& assignments)
		{
			const uword n_samples = assignments.n_cols;
			uword n_threads = std::thread::hardware_concurrency();
			if(n_threads > n_samples) n_threads = n_samples;
			cout << io::info("Number of threads used") << n_threads << endl;

			std::vector<std::future<void> > threads;
			uword chunk_size = n_samples / n_threads;
			uword c_min = 0;
			uword c_max = 0;
			for(uword t = 0; t < n_threads; ++t)
			{
				c_min = c_max;
				c_max = c_min + chunk_size;
				if(t == n_threads - 1) c_max = n_samples;
				threads.push_back(std::async(std::launch::async,
				                             &Sampler__::threaded_multi_sample,
				                             this,
				                             std::ref(assignments),
				                             c_min,
				                             c_max));
			}

			for(auto & t: threads)
				t.get();
		}

	public:
		// Public sampling operations
		inline dvec sample()
		{
			distribution__.ones();
			dvec assignment(n_literals__, arma::fill::zeros);
			std::random_device rd;
			mte gen(rd());
			generator__ = &gen;
			base_type::push_weights(edge_weights__, node_weights__, distribution__);
			sample_assignment(assignment, n_nodes__ - 1);
			generator__ = nullptr;
			return assignment;
		}

		inline dvec sample(const dvec& distribution)
		{
			assert(distribution.n_elem == n_literals__);
			std::random_device rd;
			mte gen(rd());
			generator__ = &gen;

			distribution__ = distribution;

			dvec assignment(n_literals__, arma::fill::zeros);
			base_type::push_weights(edge_weights__, node_weights__, distribution__);
			sample_assignment(assignment, n_nodes__ - 1);

			generator__ = nullptr;
			return assignment;
		}

		inline dvec sample(const dvec& dis1, const dvec& dis2, const double& gamma)
		{
			assert(dis1.n_elem == n_literals__ && dis2.n_elem == n_literals__);
			std::random_device rd;
			mte gen(rd());
			generator__ = &gen;

			std::bernoulli_distribution ber(gamma);
			if(ber(*generator__))
				distribution__ = dis1;
			else
				distribution__ = dis2;

			dvec assignment(n_literals__, arma::fill::zeros);
			base_type::push_weights(edge_weights__, node_weights__, distribution__);
			sample_assignment(assignment, n_nodes__ - 1);

			generator__ = nullptr;
			return assignment;
		}

		inline dmat sample(const uword n_samples)
		{
			distribution__.ones();
			std::random_device rd;
			mte gen(rd());
			generator__ = &gen;
			push_weights(edge_weights__, node_weights__, distribution__);
			dmat assignments(n_literals__, n_samples);
			multi_sample(assignments);
			generator__ = nullptr;
			return assignments;
		}
};

// -----------------------------------------------------------------------------
// Final class Sampler<language::dnnf>
// -----------------------------------------------------------------------------

template<>
class Sampler<language::dnnf> final: public Sampler__<language::dnnf>
{
	public:
		// Traits
		using base_type = Sampler__<language::dnnf>;

	public:
		// Constructors & Destructor
		Sampler(const Dnnf& circuit) :
			base_type(circuit)
		{
		}

		~Sampler()
		{
		}

	public:
		// Public sampling operations
		inline dvec operator()()
		{
			return sample();
		}

		inline dvec operator()(const dvec& distribution)
		{
			return sample(distribution);
		}

		inline dvec operator()(const dvec& dis1, const dvec& dis2, const double& gamma)
		{
			return sample(dis1, dis2, gamma);
		}

		inline dmat operator()(const uword n_samples)
		{
			return sample(n_samples);
		}
};

#endif
