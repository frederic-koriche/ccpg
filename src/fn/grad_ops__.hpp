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
                     const double& epsilon,
                     traits::l2)
{
	(void)epsilon;
	return assignment - weights;
}

inline dvec gradient(const dvec& assignment,
                     const dvec& weights,
                     const double& epsilon,
                     traits::ure)
{
	dvec grad(assignment.n_elem);
	for(uword x = 0; x < grad.n_elem; x++)
	{
		double v = assignment[x] + epsilon;
		double w = weights[x] + epsilon;
		grad[x] = log(v / w);
	}
	return grad;
}

#endif
