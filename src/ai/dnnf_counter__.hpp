// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// dnnf_counter__.hpp
// -----------------------------------------------------------------------------

#ifndef DNNF_COUNTER__
#define DNNF_COUNTER__

#include "dnnf_engine__.hpp"

// -----------------------------------------------------------------------------
// Abstract class Counter__<language::dnnf>
// Weighted model counter for dDNNF
// Literals weights: even index (positive literal) odd index (negative literal)
// -----------------------------------------------------------------------------

template<>
class Counter__<language::dnnf>: public Engine__<language::dnnf, query::ct>
{
	public:                 // Traits
		using base_type = Engine__<language::dnnf, query::ct>;

	protected:              // Attributes
		using base_type::circuit__;
		using base_type::n_literals__;
		using base_type::n_nodes__;
		using base_type::n_variables__;

	public:                 // Constructors & Destructor
		Counter__(const Dnnf& circuit) :
			base_type(circuit)
		{
		}

		~Counter__()
		{
		}

	public:                 // Public counting operations
		inline double count()
		{
			dvec distribution(n_literals__, arma::fill::ones);
			dvec weights(n_nodes__);
			push_weights(weights, distribution);
			return weights[n_nodes__ - 1];
		}

		inline double count(const dvec& distribution)
		{
			assert(distribution.n_elem == n_literals__);
			dvec weights(n_nodes__);
			push_weights(weights, distribution);
			return weights[n_nodes__ - 1];
		}

		inline double probability(const dvec& assignment, const dvec& distribution)
		{
			dvec weights(n_nodes__);
			push_weights(weights, distribution);
			double partition = weights[n_nodes__ - 1];
			double weight = get_weight(assignment, distribution);
			return weight / partition;
		}

		inline double probability(const init_list<Literal>& term, const dvec& distribution)
		{
			dvec weights(n_nodes__);
			dvec dis(distribution);
			push_weights(weights, dis);
			double partition = weights[n_nodes__ - 1];
			for(auto p = term.begin(); p != term.end(); ++p)
			{
				assert(p->var < n_variables__);
				if(p->sgn == 1)
					dis[(2 * p->var) + 1] = 0;
				else
					dis[2 * p->var] = 0;
			}
			push_weights(weights, dis);
			double weight = weights[n_nodes__ - 1];
			return weight / partition;
		}
};

// -----------------------------------------------------------------------------
// Final class Counter<language::dnnf>
// -----------------------------------------------------------------------------

template<>
class Counter<language::dnnf> final : public Counter__<language::dnnf>
{
	public:                 // Traits
		using base_type = Counter__<language::dnnf>;

	public:                 // Constructors & Destructor
		Counter(const Dnnf& circuit) :
			base_type(circuit)
		{
		}

		~Counter()
		{
		}

	public:                 // Public counting operator
		inline double operator()()
		{
			return count();
		}

		inline double operator()(const dvec& distribution)
		{
			return count(distribution);
		}
};

#endif
