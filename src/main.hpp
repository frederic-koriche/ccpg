// -----------------------------------------------------------------------------
//
// Online Combinatorial Optimization
// main.hpp
//
// -----------------------------------------------------------------------------

#ifndef MAIN_HPP
#define MAIN_HPP

#include "learner.hpp"

// -----------------------------------------------------------------------------
// Class Application
// Handles user input and output plots
// -----------------------------------------------------------------------------

class Application
{
	protected:
		std::vector<string> input__;
		std::vector<string> output__;
		std::bitset<4> inflags__;
		std::bitset<2> outflags__;
		uword n_trials__;
		bool is_helping__;
		bool is_running__;

	protected:
		void init(int argc, char** argv);

	public:
		// Constructors and destructors
		Application(int argc, char** argv);
		~Application() {
		}

		// Executions
		bool help();
		bool run();
};

// -----------------------------------------------------------------------------
// Initialization
// -----------------------------------------------------------------------------

Application::Application(int argc, char** argv) :
	input__(5),
	output__(2),
	inflags__(),
	outflags__(),
	n_trials__(0),
	is_helping__(false),
	is_running__(false)
{
	init(argc, argv);
}

bool Application::help()
{
	cout << "OCO version 1.0" << endl;
	cout << "oco is a framework for online combinatorial optimization" << endl;
	cout << io::title("Usage: oco [-h] -c <circuit> -l <learner> -f <feedback> -t <trials> [-p <projections>] [--regrets] [--runtimes]") << endl;
	cout << io::subsection("Positional arguments") << endl;
	cout << io::info("-c <ircuit>") << "compiled circuit in .nnf format" << endl;
	cout << io::info("-l <learner>") << "online learner in {fpl, exp_exp, omd_l2, omd_ure}" << endl;
	cout << io::info("-f <feedback>") << "environment feedback in {full, semibandit, bandit}" << endl;
	cout << io::info("-t <trials>") << "number of trials" << endl;
	cout << io::subsection("Optional arguments") << endl;
	cout << io::info("-p <projections>") << "max number of approximation steps in Bregman projection" << endl;
	cout << io::info("--regrets") << "outputs regrets plot" << endl;
	cout << io::info("--runtimes") << "outputs runtimes plot" << endl;
	cout << io::info("-h, --help") << "show this help message and exit" << endl;
	return true;
}

void Application::init(int argc, char** argv)
{
	if(argc == 1 or io::is_member(std::string(argv[1]), {"--help", "-h"}))
	{
		is_helping__ = true;
		return;
	}

	for(int i = 1; i < argc; i++)
	{
		// Positional
		string choice(argv[i]);
		if(choice == "-c" && i < argc - 1)
		{
			input__[io::circuit] = argv[i+1];
			inflags__[io::circuit] = io::is_member(io::find_extension(input__[io::circuit]), {"nnf","sdd"});
		}
		else if(choice == "-f" && i < argc - 1)
		{
			input__[io::feedback] = argv[i+1];
			inflags__[io::feedback] = io::is_member(input__[io::feedback], {"full", "semibandit", "bandit"});
		}
		else if(choice == "-l" && i < argc - 1)
		{
			input__[io::learner] = argv[i+1];
			inflags__[io::learner] = io::is_member(input__[io::learner], {"fpl", "exp_exp", "omd_l2", "omd_ure"});
		}
		else if(choice == "-t" && i < argc - 1)
		{
			input__[io::trials] = argv[i+1];
			inflags__[io::trials] = io::is_number(input__[io::trials]);
		}
		// Optional
		else if(choice == "-p" && i < argc - 1)
		{
			input__[io::projections] = argv[i+1];
		}
		else if(choice == "--regrets")
			outflags__[io::regrets] = 1;
		else if(choice == "--runtimes")
			outflags__[io::runtimes] = 1;
	}
	is_running__ = inflags__.all();
}

bool Application::run()
{
	if(is_helping__)
		return help();
	if(!is_running__)
		return false;

	if(input__[io::feedback] == "full")
	{
		Circuit<DNNF> dnnf(input__[io::circuit]);
		uword n_objectives = std::max((uword)1, n_trials__ / 10);
		uword n_trials = (uword) std::stoi(input__[io::trials]);
		Environment<DNNF,FULL> env(dnnf,n_objectives,n_trials);

		if(input__[io::learner] == "fpl")
		{
			Learner<DNNF,FPL,FULL> fpl(dnnf,env,n_trials);
			fpl.learn();
			return true;
		}

		// if(input__[io::learner] == "exp_exp")
		// {
		// 	Learner<DNNF,EXP_EXP,FULL> expExp(dnnf,env,n_trials);
		// 	expExp.learn();
		// 	return true;
		// }

		// if(input__[io::learner] == "omd_l2")
		// {
		// 	Learner<DNNF,OMD_L2,FULL> omdL2(dnnf,env,n_trials);
		// 	omdL2.learn();
		// 	return true;
		// }
	}

	return false;
}

#endif
