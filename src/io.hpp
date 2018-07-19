// -----------------------------------------------------------------------------
//
// Online Combinatorial Optimization
// io.hpp
//
// -----------------------------------------------------------------------------

#ifndef IO
#define IO

#include "traits.hpp"

namespace io
{
	enum color
	{
		black = 30,
		red = 31,
		green = 32,
		yellow = 33,
		blue = 34,
		magenta = 35,
		cyan = 36,
		white = 39
	};

// String emphasis
	enum emphasis
	{
		normal = 0,
		bold = 1,
		underline = 4
	};

// String punctuation
	enum punctuation
	{
		blank = 0,
		colon = 1,
		dot = 2
	};

	class msg;
	class info;
	class section;
	class subsection;
	class error;
	class warning;
}

#include "io/io__.hpp"

#endif
