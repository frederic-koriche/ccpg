// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// dnnf_circuit__.hpp
// -----------------------------------------------------------------------------

#ifndef DNNF_CIRCUIT__HPP
#define DNNF_CIRCUIT__HPP

#include "nnf_circuit__.hpp"
#include "dnnf_load__.hpp"

// -----------------------------------------------------------------------------
// Class Circuit<DNNF>
// (deterministic) Decomposable Negation Normal Form circuit
// Attributes range from 0 to n-1 (minus 1 from .cnf and .nnf formats)
// -----------------------------------------------------------------------------

template<>
class Circuit<DNNF>: public Circuit<NNF>
{
	public:                 // Traits
		using base_type = Circuit<NNF>;

	protected:              // Attributes
		using base_type::adjacency_matrix__;
		using base_type::edge_labels__;
		using base_type::node_labels__;
		using base_type::n_variables__;

	public:                 // Constructors and destructor
		inline Circuit() :
			base_type::Circuit()
		{
		}

		inline Circuit(uword n_nodes, uword n_variables) :
			base_type::Circuit(n_nodes, n_variables)
		{
		}

		inline Circuit(const std::string & filename) :
			Circuit()
		{
			bool is_loaded = load_nnf_file(*this, filename);
			if(!is_loaded) n_variables__ = 0;
		}

	public:                 // IO
		friend ostream & operator <<(ostream & output, const Circuit<DNNF>& circuit)
		{
			print(output, circuit);
			return output;
		}
};

#endif
