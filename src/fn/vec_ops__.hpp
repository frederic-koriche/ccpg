// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// vec_ops__.hpp
// -----------------------------------------------------------------------------

#ifndef VEC_OPS__
#define VEC_OPS__

// -----------------------------------------------------------------------------
// Vector Operators
// Basic operators on vectors
// -----------------------------------------------------------------------------

template<typename T>
inline T flip(const T& a, const double& v = 1.0)
{
	T b(a.size());
	for(uword i = 0; i < a.size(); ++i)
		b[i] = v - a[i];
	return b;
}

#endif
