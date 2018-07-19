// -----------------------------------------------------------------------------
//
// Online Combinatorial Optimization
// io__.hpp
//
// -----------------------------------------------------------------------------

#ifndef IO__
#define IO__

// -----------------------------------------------------------------------------
// STD Formating
// -----------------------------------------------------------------------------

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::ostream;
using std::string;

namespace io
{
	uword digits(uword i)
	{
		return i > 0 ? (uword)log10((double)i) + 1 : 1;
	}

	uword getw(false_type)
	{
		return 4;
	}

	uword getw(true_type)
	{
		return 8;
	}
}

ostream& operator<<(ostream& output, const upair& pa)
{
	output << "(" << pa.first << "," << pa.second << ")";
	return output;
}

template<typename T>
ostream& operator<<(ostream& output, const Array<T>& v)
{
	output.setf(std::ios::fixed, std::ios::floatfield);
	output.setf(std::ios::left);
	output.setf(std::ios::showpos);
	output.precision(4);
	uword sp = io::getw(traits::is_decimal<T>());

	for(uword i = 0; i < v.size(); ++i)
	{
		output << std::setw(sp) << v[i];
		if(std::is_fundamental<T>())
			output << " ";
		else
			output << std::endl;
	}
	return output;
}

// -----------------------------------------------------------------------------
// Output messages
// -----------------------------------------------------------------------------

namespace io
{
	class msg               // Standard msg
	{
		protected:
			std::string text__;

		public:
			msg(const std::string& text, const io::color c, const io::emphasis e, const io::punctuation p, uword s) :
				text__(text)
			{
				format_text(c, e, p, s);
			}

			msg(const std::stringstream& text, const io::color c, const io::emphasis e, const io::punctuation p, uword s) :
				text__(text.str())
			{
				format_text(c, e, p, s);
			}

			~msg()
			{
			}

		protected:
			void format_text(io::color c, io::emphasis e, io::punctuation p, uword s)
			{
				text__ += " ";
				uword t = text__.size();
				if(t < s) text__.append(s - t, '.');
				switch(p)
				{
				case blank: break;

				case colon: text__ += ": "; break;

				case dot: text__ += ".";
				}
				std::stringstream formatted;
				formatted << "\033[" << e << ";" << c << "m" << text__ << "\033[0m";
				text__ = formatted.str();
			}

		public:
			const std::string& text() const
			{
				return text__;
			}

			friend ostream& operator <<(ostream& output, const msg& m)
			{
				output << m.text();
				return output;
			}
	};

	class info : public msg         // Info msg
	{
		public:
			using base_type = msg;

			info(const std::string& text) :
				base_type(text, io::white, io::normal, io::colon, 25)
			{
			}

			info(const std::stringstream& text) :
				base_type(text, io::white, io::normal, io::colon, 25)
			{
			}
	};

	class title : public msg        // Title msg
	{
		public:
			using base_type = msg;

			title(const std::string& text) :
				base_type(text, io::magenta, io::bold, io::blank, 0)
			{
			}

			title(const std::stringstream& text) :
				base_type(text, io::magenta, io::bold, io::blank, 0)
			{
			}
	};


	class section : public msg      // Section msg
	{
		public:
			using base_type = msg;

			section(const std::string& text) :
				base_type(text, io::blue, io::bold, io::blank, 0)
			{
			}

			section(const std::stringstream& text) :
				base_type(text, io::blue, io::bold, io::blank, 0)
			{
			}
	};

	class subsection : public msg   // Title msg
	{
		using base_type = msg;

		public:
			subsection(const std::string& text) :
				base_type(text, io::yellow, io::normal, io::blank, 0)
			{
			}

			subsection(const std::stringstream & text) :
				base_type(text, io::yellow, io::normal, io::blank, 0)
			{
			}
	};
}

// -----------------------------------------------------------------------------
// Output information messages
// -----------------------------------------------------------------------------

namespace io
{
	class error                     // Error msg
	{
		protected:
			std::string header__;
			std::string text__;

		public:
			error(const std::string& header, const std::string& text, io::color c) :
				header__(header),
				text__(text)
			{
				format_header(c);
			}

			error(const std::string& header, const std::stringstream& text, io::color c) :
				header__(header),
				text__(text.str())
			{
				format_header(c);
			}

			error(const std::string& text) :
				error("error", text, io::red)
			{
			}

			error(const std::stringstream& text) :
				error("error", text, io::red)
			{
			}

			~error()
			{
			}

		protected:
			void format_header(io::color c)
			{
				header__ += ": ";
				std::stringstream formatted;
				formatted << "\033[" << emphasis::bold << ";" << c << "m" << header__ << "\033[0m";
				header__ = formatted.str();
			}

		public:
			const std::string& header() const
			{
				return header__;
			}

			const std::string& text() const
			{
				return text__;
			}

			friend ostream& operator <<(ostream& output, const error& m)
			{
				output << m.header() << m.text();
				return output;
			}
	};

	class warning : public error    // Warning msg
	{
		public:
			using base_type = error;

			warning(const std::string& text) :
				base_type("warning", text, io::cyan)
			{
			}

			warning(const std::stringstream& text) :
				base_type("warning", text, io::cyan)
			{
			}
	};
}

// -----------------------------------------------------------------------------
// File access
// -----------------------------------------------------------------------------

namespace io
{
	inline bool check_open(std::ifstream& file, const std::string& filename)
	{
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			file.open(filename);
		}
		catch(std::ifstream::failure event)
		{
			error(event.what() + filename);
			return 0;
		}
		return 1;
	}

	inline bool check_open(std::ofstream& file, const std::string& filename)
	{
		file.exceptions(std::ofstream::failbit | std::ifstream::badbit);
		try
		{
			file.open(filename);
		}
		catch(std::ofstream::failure event)
		{
			error(event.what() + filename);
			return 0;
		}
		return 1;
	}

	inline void setf(std::ostream& out)
	{
		out.setf(std::ios::fixed, std::ios::floatfield);
		out.setf(std::ios::left);
		out.setf(std::ios::showpos);
		out.precision(4);
	}
}

#endif
