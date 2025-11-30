#include <cstdlib>
#include <ctime>

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

struct parameters {
    double Power;
    double Voltage;
    double RPM;
    double Efficiency;
};

struct eval_func {
    double W_cost;
    double W_efficiency;
};
