// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// grad_ops__.hpp
// -----------------------------------------------------------------------------

#ifndef GRAD_OPS__
#define GRAD_OPS__

// -----------------------------------------------------------------------------
// Gradient operations
// -----------------------------------------------------------------------------

inline dvec gradient(const dvec& assignment,
                     const dvec& weights,
                     const double& shift,
                     traits::l2)
{
	(void)shift;
	return assignment - weights;
}

inline dvec gradient(const dvec& assignment,
                     const dvec& weights,
                     const double& shift,
                     traits::ure)
{
	dvec grad(assignment.n_elem);
	for(uword x = 0; x < grad.n_elem; x++)
	{
		double v = assignment[x] + shift;
		double w = weights[x] + shift;
		grad[x] = log(v / w);
	}
	return grad;
}

#endif
