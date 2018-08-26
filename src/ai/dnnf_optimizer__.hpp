// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// dnnf_optimizer_.hpp
// -----------------------------------------------------------------------------

#ifndef DNNF_OPTIMIZER__HPP
#define DNNF_OPTIMIZER__HPP

#include "dnnf_circuit__.hpp"

// -----------------------------------------------------------------------------
// Abstract class Optimizer__<DNNF, Q>
// Linear assignment optimizer for dDNNF
// Literals weights: even index (positive literal) odd index (negative literal)
// -----------------------------------------------------------------------------

template<query_t Q>
class Optimizer__<DNNF, Q>: public Engine__<DNNF, Q>
{
	public:                 // Traits
		using base_type = Engine__<DNNF, Q>;
		using Children = arma::SpSubview<uword>;

	protected:              // Attributes
		using base_type::circuit__;
		using base_type::n_literals__;
		using base_type::n_nodes__;
		using base_type::n_variables__;
		dvec objective__;
		sp_dmat edge_weights__;
		dvec node_weights__;

	public:                 // Constructors & Destructor
		Optimizer__(const Circuit<DNNF>& circuit) :
			base_type(circuit),
			objective__(circuit.n_literals()),
			edge_weights__(circuit.n_nodes(), circuit.n_nodes()),
			node_weights__(circuit.n_nodes())
		{
		}

		~Optimizer__()
		{
		}

	protected:              // Protected basic operations
		inline static double infinite(traits::max)
		{
			return -std::numeric_limits<double>::infinity();
		}

		inline static double infinite(traits::min)
		{
			return std::numeric_limits<double>::infinity();
		}

		inline static sword compare(const double& a, const double& b, traits::max)
		{
			if(a > b)
				return 1;
			if(a < b)
				return -1;
			return 0;
		}

		inline static sword compare(const double& a, const double& b, traits::min)
		{
			if(a < b)
				return 1;
			if(a > b)
				return -1;
			return 0;
		}

	protected:              // Protected optimization operations
		uword choose_child(const uword parent, const Children& children)
		{
			uword best_child = 0;
			double best_score = infinite(traits::to_query<Q>());
			for(auto p = children.begin(); p != children.end(); ++p)
			{
				uword child = p.col();
				double score = edge_weights__(parent, child);
				if(compare(score, best_score, traits::to_query<Q>()) > -1)
				{
					best_child = child;
					best_score = score;
				}
			}
			return best_child;
		}

		void choose_free_variables(dvec& assignment,
		                           const uword parent,
		                           const uword child)
		{
			uvec vars = circuit__.edge_label(parent, child);
			for(uword i = 0; i < vars.size(); ++i)
			{
				uword x = vars[i];
				if(compare(objective__[2 * x], objective__[(2 * x) + 1], traits::to_query<Q>()) > -1)
					assignment[2* x] = 1.0;
				else
					assignment[2* x] = 0.0;
				assignment[(2 * x) + 1] = 1.0 - assignment[2 * x];
			}
		}

		void optimize(dvec& assignment, const uword index)
		{
			char type = circuit__.node_label(index).type;

			assert(type != 'f');

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
					optimize(assignment, p.col());
				return;
			}

			if(type == 'o')
			{
				uword child = choose_child(index, children);
				choose_free_variables(assignment, index, child);
				optimize(assignment, child);
			}
		}

	public:                 // Public optimization operations
		inline dvec optimize(const dvec& objective)
		{
			assert(objective.n_elem == n_literals__);

			objective__ = objective;
			dvec assignment(n_literals__, arma::fill::zeros);
			base_type::push_weights(edge_weights__, node_weights__, objective__);
			optimize(assignment, n_nodes__ - 1);
			return assignment;
		}

		inline dvec optimize(const dvec& obj1,
		                     const dvec& obj2,
		                     const double& gamma)
		{
			assert(obj1.n_elem == n_literals__ && obj2.n_elem == n_literals__);
			std::random_device rd;
			mte gen(rd());

			std::bernoulli_distribution ber(gamma);
			if(ber(gen))
				objective__ = obj1;
			else
				objective__ = obj2;

			dvec assignment(n_literals__, arma::fill::zeros);
			base_type::push_weights(edge_weights__, node_weights__, objective__);
			optimize(assignment, n_nodes__ - 1);
			return assignment;
		}

		inline double value(const dvec& assignment, const dvec& objective)
		{
			return get_weight(assignment, objective, traits::to_query<Q>());
		}
};

// -----------------------------------------------------------------------------
// Final class Optimizer<DNNF, Q>
// -----------------------------------------------------------------------------

template<query_t Q>
class Optimizer<DNNF, Q> final : public Optimizer__<DNNF, Q>
{
	public:                  // Traits
		using base_type = Optimizer__<DNNF, Q>;

	public:                 // Constructors & Destructor
		Optimizer(const Circuit<DNNF>& circuit) :
			base_type(circuit)
		{
		}

		~Optimizer()
		{
		}

	public:                 // Public optimization operations
		inline dvec operator()(const dvec& objective)
		{
			return base_type::optimize(objective);
		}

		inline dvec operator()(const dvec& obj1, const dvec& obj2, const double& gamma)
		{
			return base_type::optimize(obj1, obj2, gamma);
		}
};

#endif
