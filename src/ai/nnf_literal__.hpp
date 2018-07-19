// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// nnf_literal__.hpp
// -----------------------------------------------------------------------------

#ifndef NNF_LITERAL__
#define NNF_LITERAL__

// -----------------------------------------------------------------------------
// Class Literal
// Boolean literal
// -----------------------------------------------------------------------------

class Literal
{
	public:                 // Attributes
		uword var;
		bool sgn;

	public:                 // Constructors & Destructor
		inline explicit Literal(uword v, bool s) :
			var(v),
			sgn(s)
		{
		}

		inline explicit Literal(const upair& p) :
			var(p.first),
			sgn((bool) p.second)
		{
		}

		inline Literal(const Literal& other) :
			var(other.var),
			sgn(other.sgn)
		{
		}

		inline Literal(Literal && other) :
			var(std::move(other.var)),
			sgn(std::move(other.sgn))
		{
		}

		virtual ~Literal()
		{
		}

	public:                 // Transformations
		Literal& operator=(const Literal& other)
		{
			var = other.var;
			sgn = other.sgn;
			return *this;
		}

		Literal& operator=(Literal&& other)
		{
			var = std::move(other.var);
			sgn = std::move(other.sgn);
			return *this;
		}

	public:                 // IO
		friend ostream & operator <<(ostream & output, const Literal& lit)
		{
			std::cout << "(" << lit.var << "," << lit.sgn << ")";
			return output;
		}
};

#endif
