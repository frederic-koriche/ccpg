// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// nnf_circuit__.hpp
// -----------------------------------------------------------------------------

#ifndef NNF_CIRCUIT__
#define NNF_CIRCUIT__

#include "nnf_literal__.hpp"
#include "nnf_node__.hpp"
#include "nnf_print__.hpp"
#include "../fn/mat_find__.hpp"

// -----------------------------------------------------------------------------
// Class Circuit<NNF>
// Negation Normal Form Circuit
// Attributes range from 0 to d-1 (minus one from convention to .cnf and .nnf)
// The number of literals is 2d i.e. 2 lit per var
// -----------------------------------------------------------------------------

template<>
class Circuit<NNF>
{
	public:                 // Traits
		using EdgeLabels = HashMap<uvec>;
		using NodeLabels = Array<Node>;

	protected:              // Attributes
		sp_umat adjacency_matrix__;
		EdgeLabels edge_labels__;
		NodeLabels node_labels__;
		uword n_variables__;

	public:                 // Constructors and destructor
		inline Circuit() :
			adjacency_matrix__(),
			edge_labels__(),
			node_labels__(),
			n_variables__(0)
		{
		}

		inline Circuit(uword n_nodes, uword n_variables) :
			adjacency_matrix__(n_nodes, n_nodes),
			edge_labels__(),
			node_labels__(n_nodes, Node()),
			n_variables__(n_variables)
		{
		}

	public:                 // Queries
		inline const arma::SpSubview<uword> in_edges(uword index) const
		{
			return adjacency_matrix__.col(index);
		}

		inline bool is_edge(uword parent, uword child) const
		{
			return (adjacency_matrix__(parent, child) == 1);
		}

		inline arma::uvec children(uword x) const
		{
			return find_cols(adjacency_matrix__.row(x));
		}

		inline const uvec& edge_label(uword parent, uword child) const
		{
			return edge_labels__.at((parent * n_nodes()) + child);
		}

		inline const EdgeLabels& edge_labels() const
		{
			return edge_labels__;
		}

		inline uword n_children(uword x) const
		{
			return adjacency_matrix__.row(x).n_nonzero;
		}

		inline uword n_edges() const
		{
			return adjacency_matrix__.n_nonzero;
		}

		inline uword n_literals() const
		{
			return 2 * n_variables__;
		}

		inline uword n_nodes() const
		{
			return node_labels__.size();
		}

		inline uword n_parents(uword x) const
		{
			return adjacency_matrix__.col(x).n_nonzero;
		}

		inline const uword& n_variables() const
		{
			return n_variables__;
		}

		inline const Node& node_label(uword index) const
		{
			return node_labels__[index];
		}

		inline const NodeLabels& node_labels() const
		{
			return node_labels__;
		}

		inline const arma::SpSubview<uword> out_edges(uword index) const
		{
			return adjacency_matrix__.row(index);
		}

		inline arma::uvec parents(uword x) const
		{
			return find_rows(adjacency_matrix__.col(x));
		}

	public:                 // Transformations
		inline void add_edge(uword parent, uword child)
		{
			adjacency_matrix__(parent, child) = 1;
		}

		inline void add_edge_label(uword parent, uword child, const uvec& label)
		{
			adjacency_matrix__(parent, child) = 1;
			edge_labels__.insert(std::make_pair((parent * n_nodes()) + child, label));
		}

		inline uvec& edge_label(uword parent, uword child)
		{
			return edge_labels__.at((parent * n_nodes()) + child);
		}

		inline EdgeLabels& edge_labels()
		{
			return edge_labels__;
		}

		inline Node& node_label(uword index)
		{
			return node_labels__[index];
		}

		inline NodeLabels& node_labels()
		{
			return node_labels__;
		}

	public:                 // IO
		friend ostream & operator <<(ostream & output, const Circuit<NNF>& circuit)
		{
			print(output, circuit);
			return output;
		}
};

#endif
