// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// nnf_node__.hpp
// -----------------------------------------------------------------------------

#ifndef NNF_NODE__
#define NNF_NODE__

// -----------------------------------------------------------------------------
// Class Node
// Node descriptor of an NNF Circuit
// -----------------------------------------------------------------------------

class Node
{
	public:                 // Attributes
		bool sgn;
		char type;
		uvec vars;

	public:                 // Constructors & Destructor
		inline Node() :
			sgn(1),
			type('a'),
			vars()
		{
		}

		inline Node(const Node& other) :
			sgn(other.sgn),
			type(other.type),
			vars(other.vars)
		{
		}

		inline Node(Node&& other) :
			sgn(std::move(other.sgn)),
			type(std::move(other.type)),
			vars(std::move(other.vars))
		{
		}

		virtual ~Node()
		{
		}

	public:                 // Transformations
		Node& operator=(const Node& other)
		{
			sgn = other.sgn;
			type = other.type;
			vars = other.vars;
			return *this;
		}

		Node& operator=(Node&& other)
		{
			sgn = std::move(other.sgn);
			type = std::move(other.type);
			vars = std::move(other.vars);
			return *this;
		}

	public:                 // IO
		friend ostream & operator <<(ostream & output, const Node& node)
		{
			output << "[type]: ";
			switch(node.type)
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
			if(node.sgn)
				output << "+1";
			else
				output << "-1";

			output << " [vars]: ";
			for(uword x = 0; x < node.vars.n_elem; ++x)
				output << node.vars[x] << " ";
			return output;
		}
};

#endif
