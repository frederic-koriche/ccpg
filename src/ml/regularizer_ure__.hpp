// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// regularizer_ure__.hpp
// -----------------------------------------------------------------------------

#ifndef REGULARIZER_URE__
#define REGULARIZER_URE__

// -----------------------------------------------------------------------------
// Regularizer<URE>
// Shifted unormalized relative entropy
// R(x) = sum_i y_i (ln y_i - 1) where y_i = x_i + shift
// -----------------------------------------------------------------------------

template<>
class Regularizer<URE>
{
	protected:
		double shift__;

	public:
		// Constructor
		Regularizer(const double& shift) :
			shift__(shift)
		{
		}

		// Destructor
		~Regularizer()
		{
		}

		// Convexity
		inline double alpha() const
		{
			return 1.0 / (1.0 + shift__);
		}

		// Smoothness
		inline double beta() const
		{
			return 1.0 / shift__;
		}

		// Gradient
		inline dvec gradient(const dvec& assignment, const dvec& weights) const
		{
			dvec grad(assignment.n_elem);
			for(uword x = 0; x < grad.n_elem; x++)
			{
				double y = assignment[x] + shift__;
				double z = weights[x] + shift__;
				grad[x] = log(y / z);
			}
			return grad;
		}

		// Value
		inline double value(const dvec& assignment) const
		{
			double val = 0;
			for(uword x = 0; x < assignment.n_elem; x++)
			{
				double y = assignment[x] + shift__;
				val += y * (log(y) - 1);
			}
			return val;
		}
};

#endif
