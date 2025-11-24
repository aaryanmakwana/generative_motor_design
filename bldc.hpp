#include <cstdlib>
#include <iostream>
#include <random>
#include <chrono>
#include <ctime>

using namespace std;

#define Kw 0.955

unsigned seed;

struct bldc_params{
  double P;
  double RPM;
  double V;
};

struct design_variables{
    double Bg;
    double ac;
    double J;
    double Bst;
    double Bsy;
    double Brt;
    double Bry;
    double lg;
    int Nspp;
    double Lt_ratio;
};

struct bldc_design{
    double Dro;
    double Dso;
    double Wrt;
    double Wst;
    double Wsy;
    double Dsh;
    double Ntp;
    double L;
    double drt;
    double Wry;
};

struct performance_params{
    double efficiency;
    double torque;
    double current;
    double copper_loss;
    double iron_loss;
};

double random_double(double lower, double upper) {
    int rd = rand();
    double rd1 = (double)rd/(double)RAND_MAX;
    rd1 = lower + (rd1*(upper-lower));
    return rd1;
}

int random_int(int lower, int upper) {
    int rd = lower + rand()%(upper-lower);
    return rd;
};

class bldc{
    public:
        struct bldc_params parameters;
        struct design_variables variables;
        struct performance_params performance;
        struct bldc_design design;

    void init(struct bldc_params params){
        parameters.V = params.V;
        parameters.P = params.P;
        parameters.RPM = params.RPM;
    }

    void guess_variables(){
        variables.Bg = random_double(0.3, 1.1);
        variables.Bst = random_double(1.1, 1.8);
        variables.Bsy = random_double(1.1, 1.8);
        variables.Brt = random_double(1.1, 1.8);
        variables.Bry = random_double(1.1, 1.8);
        variables.ac = random_double(20000.0, 40000.0);
        variables.lg = random_double(0.5, 1.3);
        variables.J = random_double(2.5, 5.5);
        variables.Nspp = random_int(1, 4);
        variables.Lt_ratio = random_double(0.5, 2.0);

        std::cout << variables.Bg << "\n";
    }
};
