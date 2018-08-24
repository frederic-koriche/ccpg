// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// dnnf_engine__.hpp
// -----------------------------------------------------------------------------

#ifndef DNNF_ENGINE__
#define DNNF_ENGINE__

#include "dnnf_circuit__.hpp"

// -----------------------------------------------------------------------------
// Abstract class Engine__<DNNF,Q>
// Inference engine for dDNNF using the push-weights dynamic programming scheme
// -----------------------------------------------------------------------------

template<query_t Q>
class Engine__<DNNF,Q>
{
	protected:              // Attributes
		const Circuit<DNNF>& circuit__;
		const uword n_literals__;
		const uword n_nodes__;
		const uword n_variables__;

	public:                 // Constructors & Destructor
		Engine__(const Circuit<DNNF>& circuit) :
			circuit__(circuit),
			n_literals__(circuit.n_literals()),
			n_nodes__(circuit.n_nodes()),
			n_variables__(circuit.n_variables())
		{
		}

		~Engine__()
		{
		}

	protected:              // Push false node
		inline void push_false_node(dvec& node_weights,
		                            const uword index,
		                            traits::ct)
		{
			node_weights[index] = 0.0;
		}

		inline void push_false_node(dvec& node_weights,
		                            const uword index,
		                            traits::min)
		{
			node_weights[index] = std::numeric_limits<double>::infinity();
		}

		inline void push_false_node(dvec& node_weights,
		                            const uword index,
		                            traits::max)
		{
			node_weights[index] = -std::numeric_limits<double>::infinity();
		}

		inline void push_false_node(dvec& node_weights, const uword index)
		{
			push_false_node(node_weights, index, traits::to_query<Q>());
		}

	protected:              // Push true node
		inline void push_true_node(dvec& node_weights,
		                           const uword index,
		                           traits::ct)
		{
			node_weights[index] = 1.0;
		}

		inline void push_true_node(dvec& node_weights,
		                           const uword index,
		                           traits::min)
		{
			node_weights[index] = 0.0;
		}

		inline void push_true_node(dvec& node_weights,
		                           const uword index,
		                           traits::max)
		{
			push_true_node(node_weights, index, traits::to_query<MIN>());
		}

		inline void push_true_node(dvec& node_weights, const uword index)
		{
			push_true_node(node_weights, index, traits::to_query<Q>());
		}

	protected:              // Push literal node
		inline void push_literal_node(dvec& node_weights,
		                              const uword index,
		                              const dvec& literal_weights)
		{
			uword x = circuit__.node_label(index).vars[0];
			if(circuit__.node_label(index).sgn)
				node_weights[index] = literal_weights[2 * x];
			else
				node_weights[index] = literal_weights[(2 * x) + 1];
		}

	protected:              // Push and node
		inline void push_and_node(dvec& node_weights,
		                          const uword index,
		                          traits::ct)
		{
			auto children = circuit__.out_edges(index);
			node_weights[index] = 1.0;
			for(auto c = children.begin(); c != children.end(); ++c)
				node_weights[index] *= node_weights[c.col()];
		}

		inline void push_and_node(dvec& node_weights,
		                          const uword index,
		                          traits::min)
		{
			auto children = circuit__.out_edges(index);
			node_weights[index] = 0.0;
			for(auto c = children.begin(); c != children.end(); ++c)
				node_weights[index] += node_weights[c.col()];
		}

		inline void push_and_node(dvec& node_weights,
		                          const uword index,
		                          traits::max)
		{
			push_and_node(node_weights, index, traits::to_query<MIN>());
		}

		inline void push_and_node(dvec& node_weights, const uword index)
		{
			push_and_node(node_weights, index, traits::to_query<Q>());
		}

	protected:              // Push or node
		inline void push_or_node(dvec& node_weights,
		                         const uword index,
		                         const dvec& literal_weights,
		                         traits::ct)
		{
			auto children = circuit__.out_edges(index);
			node_weights[index] = 0.0;
			for(auto c = children.begin(); c != children.end(); ++c)
			{
				uword child = c.col();
				const uvec& vars = circuit__.edge_label(index, child);
				double edge_weight = 1;
				for(uword i = 0; i < vars.size(); ++i)
				{
					uword x = vars[i];
					double w = literal_weights[2 * x] + literal_weights[(2 * x) + 1];
					edge_weight *= w;
				}
				node_weights[index] += (edge_weight * node_weights[child]);
			}
		}

		inline void push_or_node(sp_dmat& edge_weights,
		                         dvec& node_weights,
		                         const uword index,
		                         const dvec& literal_weights,
		                         traits::ct)
		{
			auto children = circuit__.out_edges(index);
			node_weights[index] = 0.0;
			for(auto c = children.begin(); c != children.end(); ++c)
			{
				uword child = c.col();
				const uvec& vars = circuit__.edge_label(index, child);
				double edge_weight = 1;
				for(uword i = 0; i < vars.size(); ++i)
				{
					uword x = vars[i];
					double w = literal_weights[2 * x] + literal_weights[(2 * x) + 1];
					edge_weight *= w;
				}
				edge_weights(index, child) = (edge_weight * node_weights[child]);
				node_weights[index] += (edge_weight * node_weights[child]);
			}
		}

		inline void push_or_node(sp_dmat& edge_weights,
		                         dvec& node_weights,
		                         const uword index,
		                         const dvec& literal_weights,
		                         traits::min)
		{
			auto children = circuit__.out_edges(index);
			node_weights[index] = std::numeric_limits<double>::infinity();
			for(auto c = children.begin(); c != children.end(); ++c)
			{
				uword child = c.col();
				const uvec& vars = circuit__.edge_label(index, child);
				double edge_weight = 0;
				for(uword i = 0; i < vars.size(); ++i)
				{
					uword x = vars[i];
					double w = std::min(literal_weights[2 * x], literal_weights[(2 * x) + 1]);
					edge_weight += w;
				}
				edge_weights(index, child) = edge_weight + node_weights[child];
				node_weights[index] = std::min(node_weights[index], edge_weight + node_weights[child]);
			}
		}

		inline void push_or_node(sp_dmat& edge_weights,
		                         dvec& node_weights,
		                         const uword index,
		                         const dvec& literal_weights,
		                         traits::max)
		{
			auto children = circuit__.out_edges(index);
			node_weights[index] = -std::numeric_limits<double>::infinity();
			for(auto c = children.begin(); c != children.end(); ++c)
			{
				uword child = c.col();
				const uvec& vars = circuit__.edge_label(index, child);
				double edge_weight = 0;
				for(uword i = 0; i < vars.size(); ++i)
				{
					uword x = vars[i];
					double w = std::max(literal_weights[2 * x], literal_weights[(2 * x) + 1]);
					edge_weight += w;
				}
				edge_weights(index, child) = edge_weight + node_weights[child];
				node_weights[index] = std::max(node_weights[index], edge_weight + node_weights[child]);
			}
		}


		inline void push_or_node(dvec& node_weights,
		                         const uword index,
		                         const dvec& literal_weights)
		{
			push_or_node(node_weights, index, literal_weights, traits::to_query<Q>());
		}

		inline void push_or_node(sp_dmat& edge_weights,
		                         dvec& node_weights,
		                         const uword index,
		                         const dvec& literal_weights)
		{
			push_or_node(edge_weights, node_weights, index, literal_weights, traits::to_query<Q>());
		}

	protected:              // get_weight
		inline static double get_weight(const dvec& assignment,
		                                const dvec& objective,
		                                traits::min)
		{
			assert(objective.n_elem == assignment.n_elem);
			return arma::dot(assignment,objective);
		}

		inline static double get_weight(const dvec& assignment,
		                                const dvec& objective,
		                                traits::max)
		{
			return get_weight(assignment,objective,traits::to_query<MIN>());
		}

		inline static double get_weight(const dvec& assignment,
		                                const dvec& objective,
		                                traits::ct)
		{
			assert(objective.n_elem == assignment.n_elem);
			double w = 1;
			for(uword x = 0; x < assignment.n_elem; ++x)
				if(assignment[x] == 1.0)
					w *= objective[x];
			return w;
		}

	public:                 // public inference operations
		inline static double get_weight(const dvec& assignment, const dvec& objective)
		{
			return get_weight(assignment,objective,traits::to_query<Q>());
		}

		inline void push_weights(dvec& node_weights, const dvec& literal_weights)
		{
			for(uword index = 0; index < n_nodes__; ++index)
				switch(circuit__.node_label(index).type)
				{
				case 'a':
					push_and_node(node_weights, index);
					break;

				case 'f':
					push_false_node(node_weights, index);
					break;

				case 'l':
					push_literal_node(node_weights, index, literal_weights);
					break;

				case 'o':
					push_or_node(node_weights, index, literal_weights);
					break;

				case 't':
					push_true_node(node_weights, index);
					break;
				}
		}

		inline void push_weights(sp_dmat& edge_weights, dvec& node_weights, const dvec& literal_weights)
		{
			for(uword index = 0; index < n_nodes__; ++index)
				switch(circuit__.node_label(index).type)
				{
				case 'a':
					push_and_node(node_weights, index);
					break;

				case 'f':
					push_false_node(node_weights, index);
					break;

				case 'l':
					push_literal_node(node_weights, index, literal_weights);
					break;

				case 'o':
					push_or_node(edge_weights, node_weights, index, literal_weights);
					break;

				case 't':
					push_true_node(node_weights, index);
					break;
				}
		}
};

#endif
