#include "learner.hpp"

//using namespace arma;
using namespace std;

int main(int argc, char **argv)
{
	cout << "hello world" << endl;

	Dnnf dnnf("tire-3.nnf");
	//cout << io::info("Dnnf") << endl << dnnf << endl;
	//Counter<language::dnnf> counter(dnnf);
	//cout << io::info("#models") << counter.count() << endl;
	//Sampler<language::dnnf> sampler(dnnf);
	//cout << io::info("Ten Samples") << endl << sampler(10) << endl;
	//UnivariateMarginalizer<language::dnnf> marginalizer(dnnf);
	//cout << io::info("Marginalization") << endl << marginalizer() << endl;
	//dvec objective(dnnf.n_literals(), arma::fill::ones);
	//Optimizer<language::dnnf,query::min> optimizer(dnnf);
	//cout << io::info("Optimization") << endl << optimizer(objective) << endl;
	Environment<language::dnnf, feedback::full> env(dnnf,10,100);
	cout << io::info("Environment") << endl << env << endl;
	Learner<language::dnnf, feedback::full, strategy::exp> learner(dnnf,env,100);
	learner.learn();
	//
	// // Dnnf dnnf("aim-50-1_6-yes1-4.nnf");
	// //Dnnf dnnf("test_03.nnf");
	// // Environment<language::dnnf,feedback::semibandit> env(dnnf);
	// //
	// // cout << io::title("Bandit learning with exp") << endl;
	// // Learner<language::dnnf,feedback::semibandit,strategy::ure> learner(dnnf, env, 1000);
	// // learner.learn();
	//
	// // Sdd sdd("sat-grid-pbl-0010.sdd");
	// ////Sdd sdd("aim-50-1_6-yes1-4.sdd");
	// ////Sdd sdd("test_02.sdd");
	// // cout << Msg("Sdd") << endl << sdd << endl;
	// // Counter<language::sdd> c2(sdd);
	// // cout << Msg("Counting") << c2.count() << endl;
	// // DnnfSampler sampler(m);
	// // cout << Msg("Counting") << endl << sampler.sample(10) << endl;
}
