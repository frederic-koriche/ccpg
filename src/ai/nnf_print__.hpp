// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// nnf_print__.hpp
// -----------------------------------------------------------------------------

#ifndef NNF_PRINT__HPP
#define NNF_PRINT__HPP

// -----------------------------------------------------------------------------
// Print Circuits
// -----------------------------------------------------------------------------

template<circuit_t C>
void print(ostream& output, const Circuit<C>& circuit)
{
	output.setf(std::ios::fixed, std::ios::floatfield);
	output.setf(std::ios::left);
	output.setf(std::ios::showpos);
	output.precision(4);

	output << io::subsection("Statistics") << endl;
	output << io::info("Variables") << circuit.n_variables() << std::endl;
	output << io::info("Vertices") << circuit.n_nodes() << std::endl;
	output << io::info("Edges") << circuit.n_edges() << std::endl;

	output << io::subsection("Description") << endl;
	uword sp = io::digits(circuit.n_nodes()) + 1;
	for(uword i = 0; i < circuit.n_nodes(); ++i)
	{
		output << std::setw(sp) << i << circuit.node_label(i);
		uvec children = circuit.children(i);
		if(!children.empty())
		{
			output << "[children] ";
			for(uword j = 0; j < children.n_elem; ++j)
				output << children[j] << " ";
		}
		output << endl;
	}
}

#endif
