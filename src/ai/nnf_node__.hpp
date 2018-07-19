// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// nnf_node__.hpp
// -----------------------------------------------------------------------------

#ifndef NNF_NODE__
#define NNF_NODE__

// -----------------------------------------------------------------------------
// Class Node<language::nnf>
// Node descriptor of an NNF Circuit
// -----------------------------------------------------------------------------

template<>
class Node<language::nnf>
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
		friend ostream & operator <<(ostream & output, const Node<language::nnf>& node)
		{
			print(output, node);
			return output;
		}
};

#endif
