// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// regularizer_l2_.hpp
// -----------------------------------------------------------------------------

#ifndef REGULARIZER_L2__
#define REGULARIZER_L2__

// -----------------------------------------------------------------------------
// Regularizer<L2>
// Tychonov regularizer
// R(x) = (1/2) || x ||_2^2
// -----------------------------------------------------------------------------

template<>
class Regularizer<L2>
{
	public:
		// Constructor
		Regularizer()
		{
		}

		// Destructor
		~Regularizer()
		{
		}

		// Convexity
		inline static double alpha()
		{
			return 1.0;
		}

		// Smoothness
		inline static double beta()
		{
			return 1.0;
		}

		// Gradient
		inline static dvec gradient(const dvec& assignment, const dvec& weights)
		{
			return assignment - weights;
		}

		// Value
		inline static double value(const dvec& assignment)
		{
			return 0.5 * arma::norm(assignment, 2);
		}
};

#endif
