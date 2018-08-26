// -----------------------------------------------------------------------------
// Online Combinatorial Optimization
// dnnf_load__.hpp
// -----------------------------------------------------------------------------

#ifndef DNNF_LOAD__HPP
#define DNNF_LOAD__HPP

#include "../fn/set_ops__.hpp"

// -----------------------------------------------------------------------------
// Load operations
// -----------------------------------------------------------------------------

template<circuit_t C>
bool load_nnf_and_node(Circuit<C>& circuit, const std::string& buffer, const uword node)
{
	//cout << io::info("reading and node") << "[" << node << "]: " << buffer << endl;

	std::regex pattern_true {R"(^(\s*[A])(\s+[0]))"};
	std::smatch match;
	if(std::regex_match(buffer, match, pattern_true))
	{
		circuit.node_label(node).type = 't';
		return 1;
	}

	std::regex pattern_and {R"((\s*[A])(\s+\d+)((\s|\d)*))"};
	if(!std::regex_match(buffer, match, pattern_and))
	{
		cerr << io::error("bad format in " + buffer) << endl;
		return 0;
	}

	uword n_children = (uword)std::stoi(match[2]);
	std::string str_children = match[3];
	std::regex pattern_child {R"(\d+)"};
	std::sregex_iterator pos(str_children.cbegin(), str_children.cend(), pattern_child);
	std::sregex_iterator end;

	uvec vars;
	uword n = 0;
	while(pos != end)
	{
		uword child = (uword)std::stoi(pos->str());
		circuit.add_edge(node, child);
		if(!circuit.node_label(child).vars.empty())
			vars.insert_rows(vars.n_elem, circuit.node_label(child).vars);
		pos++;
		n++;
	}

	if(n != n_children)
	{
		cerr << io::error("bad number of children in" + buffer) << endl;
		return 0;
	}

	circuit.node_label(node).type = 'a';
	circuit.node_label(node).vars = arma::sort(vars);
	return 1;
}

template<circuit_t C>
bool load_nnf_or_node(Circuit<C>& circuit, const std::string& buffer, const uword node)
{
	//cout << io::info("reading or node") << "[" << node << "]: " << buffer << endl;

	std::regex pattern_false {R"(^(\s*[O])(\s+0)(\s+[0]))"};
	std::smatch match;
	if(std::regex_match(buffer, match, pattern_false))
	{
		circuit.node_label(node).type = 'f';
		return 1;
	}

	std::regex pattern_or {R"(^(\s*\w+)(\s+\d+)(\s+\d+)(\s+\d+)(\s+\d+))"};
	if(!std::regex_match(buffer, match, pattern_or))
	{
		cerr << io::error("bad format in " + buffer) << endl;
		return 0;
	}

	uword left = (uword)std::stoi(match[4]);
	uword right = (uword)std::stoi(match[5]);
	circuit.add_edge(node, left);
	circuit.add_edge(node, right);
	circuit.node_label(node).type = 'o';
	circuit.node_label(node).vars = set_union(circuit.node_label(left).vars, circuit.node_label(right).vars);
	circuit.add_edge_label(node, left, set_difference(circuit.node_label(node).vars, circuit.node_label(left).vars));
	circuit.add_edge_label(node, right, set_difference(circuit.node_label(node).vars, circuit.node_label(right).vars));
	return 1;
}

template<circuit_t C>
bool load_nnf_literal(Circuit<C>& circuit, const std::string& buffer, const uword node)
{
	//cout << io::info("reading literal") << "[" << node << "]: " << buffer << endl;

	std::regex pattern_literal {R"(^(\s*[L])(\s+-?\d+))"};
	std::smatch match;
	if(!std::regex_match(buffer, match, pattern_literal))
	{
		cerr << io::error("bad format in " + buffer) << endl;
		return 0;
	}

	sword lit = (sword)std::stoi(match[2]);
	if(lit == 0)
	{
		cerr << io::error("literal with variable 0 in " + buffer) << endl;
		return 0;
	}

	circuit.node_label(node).type = 'l';
	circuit.node_label(node).sgn = ((lit > 0) ? true : false);
	circuit.node_label(node).vars.set_size(1);
	circuit.node_label(node).vars[0] = std::abs(lit) - 1;
	return 1;
}

bool load_nnf_header(uword& n_edges, uword& n_nodes, uword& n_variables, const std::string& buffer)
{
	//cout << io::info("reading header") << buffer << endl;

	std::regex pattern_header {R"(^(\s*nnf|Nnf)(\s+\d+)(\s+\d+)(\s+\d+))"};
	std::smatch match;
	if(!std::regex_match(buffer, match, pattern_header))
	{
		cerr << io::error("bad format in " + buffer) << endl;
		return 0;
	}

	n_nodes = (uword)std::stoi(match[2]);
	n_edges = (uword)std::stoi(match[3]);
	n_variables = (uword)std::stoi(match[4]);
	//cout << io::info("header") << "[nodes]: " << n_nodes << " [edges]: " << n_edges << " [variables]: " << n_variables << endl;
	return 1;
}

template<circuit_t C>
bool load_nnf_file(Circuit<C>& circuit, const std::string& dataset_name)
{
	cout << io::info("loading file") << dataset_name << endl;

	std::string filename = "./dat/" + dataset_name;
	std::ifstream file;
	if(!io::check_open(file, filename))
	{
		cerr << io::error("cannot open " + filename) << endl;
		return 0;
	}
	file.exceptions(std::ifstream::badbit);

	try
	{
		std::string buffer;
		uword node = 0;
		bool is_first = true;
		bool is_reading = true;
		uword n_edges = 0;
		uword n_nodes = 0;
		uword n_variables = 0;

		while(is_reading and std::getline(file, buffer))
		{
			if(is_first)
			{
				is_reading = load_nnf_header(n_edges, n_nodes, n_variables, buffer);
				if(is_reading)
					circuit = Circuit<C>(n_nodes, n_variables);
				is_first = false;
			}
			else
			{
				char head = buffer[0];
				switch(head)
				{
				case 'A':
					is_reading = load_nnf_and_node(circuit, buffer, node);
					break;

				case 'L':
					is_reading = load_nnf_literal(circuit, buffer, node);
					break;

				case 'O':
					is_reading = load_nnf_or_node(circuit, buffer, node);
				}
				node++;
			}
		}
	}
	catch(std::ifstream::failure event)
	{
		io::error("unable to read file " + filename + "; cause: " + event.what());
		return 0;
	}

	file.close();
	return 1;
}


#endif
