// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// mat_find__.hpp
// -----------------------------------------------------------------------------

#ifndef MAT_FIND__
#define MAT_FIND__

// -----------------------------------------------------------------------------
// Mat Find
// Find operations in matrices
// -----------------------------------------------------------------------------

template<typename T>
inline arma::uvec find_cols(const arma::SpSubview<T>& m)
{
	arma::uvec v(m.n_nonzero);
	uword x = 0;
	for(auto p = m.begin(); p != m.end(); ++p)
		v[x++] = p.col();
	return v;
}

template<typename T>
inline arma::uvec find_rows(const arma::SpSubview<T>& m)
{
	arma::uvec v(m.n_nonzero);
	uword x = 0;
	for(auto p = m.begin(); p != m.end(); ++p)
		v[x++] = p.row();
	return v;
}

#endif
