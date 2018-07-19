// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// nnf_print__.hpp
// -----------------------------------------------------------------------------

#ifndef NNF_PRINT__
#define NNF_PRINT__

// -----------------------------------------------------------------------------
// Print Node Label
// -----------------------------------------------------------------------------

template<language L>
void print(ostream& output, const Node<L>& node_label)
{
	output << "[type]: ";
	switch(node_label.type)
	{
	case 'a':
		output << "and  ";
		break;

	case 'd':
		output << "dec  ";
		break;

	case 'f':
		output << "false";
		break;

	case 'l':
		output << "lit  ";
		break;

	case 'o':
		output << "or   ";
		break;

	case 't':
		output << "true ";
		break;
	}

	output << " [sgn]: ";
	if(node_label.sgn)
		output << "+1";
	else
		output << "-1";

	output << " [vars]: ";
	for(uword x = 0; x < node_label.vars.n_elem; ++x)
		output << node_label.vars[x] << " ";
}

// -----------------------------------------------------------------------------
// Print Circuit
// -----------------------------------------------------------------------------

template<language L>
void print(ostream& output, const Circuit<L>& circuit)
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
