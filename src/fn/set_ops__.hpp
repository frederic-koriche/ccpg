// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// set_ops__.hpp
// -----------------------------------------------------------------------------

#ifndef SET_OPS__HPP
#define SET_OPS__HPP

// -----------------------------------------------------------------------------
// Set Operators
// Basic operators of set theory
// -----------------------------------------------------------------------------

template<typename T>
inline bool member(const typename T::elem_type& x, const T& a)
{
	return std::binary_search(a.cbegin(), a.cend(), x);
}

template<typename T>
inline bool includes(const T& a, const T& b)
{
	return std::includes(a.cbegin(), a.cend(), b.cbegin(), b.cend());
}

template<typename T>
inline T merge(const T& a, const T& b)
{
	T c(a.size() + b.size());
	std::merge(a.cbegin(), a.cend(), b.cbegin(), b.cend(), c.begin());
	return c;
}

template<typename T>
inline T set_difference(const T& a, const T& b)
{
	T c(a.size() + b.size());
	typename T::iterator pos = std::set_difference(a.cbegin(), a.cend(), b.cbegin(), b.cend(), c.begin());
	uword s = pos - c.begin();
	if(s == 0)
	{
		T d;
		return d;
	}
	T d(c.memptr(), s);
	return d;
}

template<typename T>
inline T set_intersection(const T& a, const T& b)
{
	T c(a.size() + b.size());
	typename T::iterator pos = std::set_intersection(a.cbegin(), a.cend(), b.cbegin(), b.cend(), c.begin());
	uword s = pos - c.begin();
	T d(c.memptr(), s);
	return d;
}

template<typename T>
inline T set_symmetric_difference(const T& a, const T& b)
{
	T c(a.size() + b.size());
	typename T::iterator pos = std::set_symmetric_difference(a.cbegin(), a.cend(), b.cbegin(), b.cend(), c.begin());
	uword s = pos - c.begin();
	T d(c.memptr(), s);
	return d;
}

template<typename T>
inline T set_union(const T& a, const T& b)
{
	T c(a.size() + b.size());
	typename T::iterator pos = std::set_union(a.cbegin(), a.cend(), b.cbegin(), b.cend(), c.begin());
	uword s = pos - c.begin();
	T d(c.memptr(), s);
	return d;
}

#endif
