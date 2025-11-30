#include <cstdlib>
#include <iostream>
#include <random>
#include <chrono>
#include <ctime>

using namespace std;

#define Kw 0.955
#define PI 3.14159265358979323846
#define f 50
#define nPHASES 3
#define RESISTIVITY 0.0000002
#define SPECIFIC_IRON_LOSS 0.02

struct fitness_function{
    double W_eff;
    double W_T;
    double W_op;
};

struct bldc_params{
  double P;
  double RPM;
  double V;
  double efficiency;
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
    double Kmag;
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
    double d_mag;
    double mag_arc;
    double Tphase;
};

struct performance_params{
    double efficiency;
    double torque;
    double current;
    double copper_loss;
    double iron_loss;
};

double random_double(double, double);
void design_variables(class bldc);
int random_int(int,int);

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

class BLDC{
    public:
        struct bldc_params parameters;
        struct design_variables variables;
        struct performance_params performance;
        struct bldc_design design;

    void init(struct bldc_params params){
        parameters.V = params.V;
        parameters.P = params.P;
        parameters.RPM = params.RPM;
        parameters.efficiency = params.efficiency;
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
        variables.Kmag = random_double(0.5, 1.0);
        //print_variables();
    }

    void print_variables(){
        std::cout << "Bg : " << variables.Bg << ",\t";
        std::cout << "Bst : " << variables.Bst << ",\t";
        std::cout << "Bsy : " << variables.Bsy << ",\t";
        std::cout << "Brt : " << variables.Brt << ",\t";
        std::cout << "Bry : " << variables.Bry << ",\t";
        std::cout << "ac : " << variables.ac << ",\t";
        std::cout << "lg : " << variables.lg << ",\t";
        std::cout << "J : " << variables.J << ",\t";
        std::cout << "Nspp : " << variables.Nspp << ",\t";
        std::cout << "Lt_ratio : " << variables.Lt_ratio << "\t";
    }

    void print_performance(){
        std::cout << "efficiency : " << performance.efficiency << ",\t";
        std::cout << "copper_loss : " << performance.copper_loss << ",\t";
        std::cout << "torque : " << performance.torque << ",\t";
        std::cout << "iron_loss : " << performance.iron_loss << ",\t";
        std::cout << "current : " << performance.current << "\t";
    }

    void print_all(){
        std::cout<< "[ ";
        print_variables();
        std::cout<< ",\t";
        print_performance();
        std::cout<< " ]\n";
    }

    double fitness(struct fitness_function ff){
        return ((ff.W_T * performance.torque) + (ff.W_eff * performance.efficiency) + (ff.W_op * performance.current * parameters.V))/(ff.W_eff + ff.W_op + ff.W_T);
    }

};

void design_bldc(class bldc * bldc){
    double Ktrv = 2 * bldc->variables.ac * bldc->variables.Bg;
    double T = (60 * bldc->parameters.P) / (bldc->parameters.efficiency * bldc->parameters.RPM * 2 * PI);
    double nPoles = 120 * f / bldc->parameters.RPM;
    double Ld_ratio = bldc->variables.Lt_ratio * PI / nPoles;

    bldc->design.Dro = std::pow(4*T/(PI * Ktrv * Ld_ratio), (1/3));
    bldc->design.L = bldc->design.Dro * Ld_ratio;

    double Nslots = bldc->variables.Nspp * nPHASES * nPoles;

    bldc->performance.current = (bldc->parameters.P * 1000) / (bldc->parameters.efficiency * bldc->parameters.V * 2);

    bldc->design.Tphase = (int)(PI*bldc->design.Dro*bldc->variables.ac) / (int)( 1000 * bldc->performance.current);
    int Z = 2*bldc->design.Tphase;

    double Ac = bldc->performance.current / bldc->variables.J;
    double Aslot = Ac*Z;

    bldc->design.Wst = (bldc->variables.Bg * PI * bldc->design.Dro / nPoles) / (bldc->variables.Bst * bldc->design.L);
    double Wslot = (PI * bldc->design.Dro / Nslots) - bldc->design.Wst;
    double d_slot = Aslot/Wslot;

    bldc->design.Wsy = (bldc->variables.Bg * PI * bldc->design.Dro / nPoles) / (2 * bldc->variables.Bsy * bldc->design.L);
    bldc->design.d_mag = bldc->variables.lg * random_double(5.0, 11.0);

    bldc->design.Dso = bldc->design.Dro + (2*bldc->design.Wsy) + (2*d_slot);
    bldc->design.mag_arc = (360/nPoles) * bldc->variables.Kmag;
    bldc->design.Wry   = (bldc->variables.Bg * PI * bldc->design.Dro / nPoles) / (2 * bldc->variables.Bry * bldc->design.L);
    bldc->design.Dsh = bldc->design.Dro - (2*bldc->design.d_mag) - (2*bldc->design.Wry);

    bldc->performance.torque = T;
    /*
    while (bldc->design.Dsh < 0) {
        std::cout << "Design failed, retrying..." << std::endl;
        bldc->design.d_mag *= 0.9;
        bldc->design.Wry *= 0.9;
    }
    */
}

void estimate_performance(class bldc * bldc) {
    bldc->performance.copper_loss = std::pow(bldc->variables.ac * bldc->design.Dro * PI, 2) * (bldc->design.Tphase * bldc->design.L * nPHASES * RESISTIVITY);
    bldc->performance.iron_loss = 0.25*PI*(std::pow(bldc->design.Dso, 2) - std::pow(bldc->design.Dro, 2)) * bldc->design.L * SPECIFIC_IRON_LOSS;
    double power = 3 * bldc->parameters.V * bldc->performance.current;
    bldc->performance.efficiency = power / (power + bldc->performance.copper_loss + bldc->performance.iron_loss);
    //bldc->print_all();

    /*
    std::cout << "[" ;
    bldc->print_performance();
    std::cout << "]\n" ;
    */
}


bldc breed(bldc parent1, bldc parent2) {
    bldc child;
    child.parameters = parent1.parameters;
    child.variables.Bg = (rand()%2 == 0) ? parent1.variables.Bg : parent2.variables.Bg;
    child.variables.Bst = (rand()%2 == 0) ? parent1.variables.Bst : parent2.variables.Bst;
    child.variables.Bsy = (rand()%2 == 0) ? parent1.variables.Bsy : parent2.variables.Bsy;
    child.variables.Brt = (rand()%2 == 0) ? parent1.variables.Brt : parent2.variables.Brt;
    child.variables.Bry = (rand()%2 == 0) ? parent1.variables.Bry : parent2.variables.Bry;
    child.variables.J = (rand()%2 == 0) ? parent1.variables.J : parent2.variables.J;
    child.variables.Kmag = (rand()%2 == 0) ? parent1.variables.Kmag : parent2.variables.Kmag;
    child.variables.Lt_ratio = (rand()%2 == 0) ? parent1.variables.Lt_ratio : parent2.variables.Lt_ratio;
    child.variables.Nspp = (rand()%2 == 0) ? parent1.variables.Nspp : parent2.variables.Nspp;
    child.variables.ac = (rand()%2 == 0) ? parent1.variables.ac : parent2.variables.ac;
    child.variables.lg = (rand()%2 == 0) ? parent1.variables.lg : parent2.variables.lg;
    return child;
}
