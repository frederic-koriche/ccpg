#include "learner.hpp"

//using namespace arma;
//using namespace std;

int main(int argc, char **argv)
{
	Circuit<DNNF> dnnf("test_01.nnf");
	cout << io::info("Circuit<DNNF>") << endl << dnnf << endl;
	Counter<DNNF> counter(dnnf);
	cout << io::info("#models") << counter.count() << endl;
	Environment<DNNF,FULL> env(dnnf,1,100);
	cout << io::info("Environment") << endl << env << endl;
	Learner<DNNF,SGD,FULL> sgd(dnnf,env,100);
	sgd.learn();
	Learner<DNNF,EXPEXP,FULL> hedge(dnnf,env,100);
	hedge.learn();

	// Regularizer<URE> reg(0.1);
	// Projector<DNNF,PCG,URE> project(dnnf,reg,50);
	// dvec distribution(dnnf.n_literals(), arma::fill::ones);
	// double epsilon = 0.1;
	// dvec prediction = project(distribution, epsilon);
	// cout << io::info("Prediction") << endl << prediction << endl;
	//Sampler<DNNF> sampler(dnnf);
	//cout << io::info("Ten Samples") << endl << sampler(10) << endl;
	// UnivariateMarginalizer<DNNF> marginalizer(dnnf);
	// cout << io::info("Marginalization") << endl << marginalizer() << endl;
	// dvec objective(dnnf.n_literals(), arma::fill::ones);
	// Optimizer<DNNF,Min> optimizer(dnnf);
	// cout << io::info("Optimization") << endl << optimizer(objective) << endl;
	// Environment<DNNF, feedback::full> env(dnnf,1,100);
	// cout << io::info("Environment") << endl << env << endl;
	// Learner<DNNF, feedback::full, strategy::fpl> learner(dnnf,env,100);
	// learner.learn();
	//
	// // Circuit<DNNF> dnnf("aim-50-1_6-yes1-4.nnf");
	// //Circuit<DNNF> dnnf("test_03.nnf");
	// // Environment<DNNF,feedback::semibandit> env(dnnf);
	// //
	// // cout << io::title("Bandit learning with exp") << endl;
	// // Learner<DNNF,feedback::semibandit,strategy::ure> learner(dnnf, env, 1000);
	// // learner.learn();
	//
	// // Sdd sdd("sat-grid-pbl-0010.sdd");
	// ////Sdd sdd("aim-50-1_6-yes1-4.sdd");
	// ////Sdd sdd("test_02.sdd");
	// // cout << Msg("Sdd") << endl << sdd << endl;
	// // Counter<language::sdd> c2(sdd);
	// // cout << Msg("Counting") << c2.count() << endl;
	// // Circuit<DNNF>Sampler sampler(m);
	// // cout << Msg("Counting") << endl << sampler.sample(10) << endl;
}
