#include "random_btw.hpp"
#include <cstdlib>
#include <iostream>
#include <random>
#include <chrono>
#include <ctime>

using namespace std;

struct fitness_function{
    double W_eff;
    double W_T;
    double W_op;
};

class SRM{

    struct srm_design_variables{
        double Bg;
        double ac;
        double J;
        double Bst;
        double Bsy;
        double Brt;
        double Bry;
        double lg;
        double k1;
        double k2;
        double ke;
        double kd;
        double LD_ratio;
    } variables;

    struct srm_design{
        double Dro;
        double Dso;
        double Wrt;
        double Wry;
        double Wst;
        double Wsy;
        double Dsh;
        double Ns;
        double Nr;
        double Bs;
        double Br;
        double L;
        double drt;
        double dst;
        double Tphase;
    } design;

    struct performance_params{
        double efficiency;
        double torque;
        double current;
        double copper_loss;
        double iron_loss;
        double cost;
    } performance;


    public:

    void guess_variables(struct parameters P){
        variables.Bg = random_double(0.01, 0.1);
        variables.ac = random_double(0.01, 0.1);
        variables.J = random_double(0.01, 0.1);
        variables.Bst = random_double(0.01, 0.1);
        variables.Bsy = random_double(0.01, 0.1);
        variables.Brt = random_double(0.01, 0.1);
        variables.Bry = random_double(0.01, 0.1);
        variables.lg = random_double(0.01, 0.1);
        variables.k1 = random_double(0.01, 0.1);
        variables.k2 = random_double(0.01, 0.1);
        variables.ke = random_double(0.01, 0.1);
        variables.kd = random_double(0.01, 0.1);
        variables.LD_ratio = random_double(0.01, 0.1);
    }

    void print_variables(){
        std::cout<< "[ Bg: "<< variables.Bg << ", ";
        std::cout<< "ac: "<< variables.ac << ", ";
        std::cout<< "J: "<< variables.J << ", ";
        std::cout<< "Bst: "<< variables.Bst << ", ";
        std::cout<< "Bsy: "<< variables.Bsy << ", ";
        std::cout<< "Brt: "<< variables.Brt << ", ";
        std::cout<< "Bry: "<< variables.Bry << ", ";
        std::cout<< "lg: "<< variables.lg << ", ";
        std::cout<< "k1: "<< variables.k1 << ", ";
        std::cout<< "k2: "<< variables.k2 << ", ";
        std::cout<< "ke: "<< variables.ke << ", ";
        std::cout<< "kd: "<< variables.kd << ", ";
        std::cout<< "LD_ratio: "<< variables.LD_ratio << " ]\n"<< std::endl;
    }

    void print_design(){
        std::cout<< "[ Dro: "<< design.Dro << ", ";
        std::cout<< "Dso: "<< design.Dso << ", ";
        std::cout<< "Wrt: "<< design.Wrt << ", ";
        std::cout << "Wst: "<< design.Wst << ", ";
        std::cout << "Wry: "<< design.Wry << ", ";
        std::cout << "Wsy: "<< design.Wsy << ", ";
        std::cout << "Wsy: "<< design.Wsy << ", ";
        std::cout << "Dsh: "<< design.Dsh << ", ";
        std::cout << "Ns: "<< design.Ns << ", ";
        std::cout << "Nr: "<< design.Nr << ", ";
        std::cout << "Bs: "<< design.Bs << ", ";
        std::cout << "Br: "<< design.Br << ", ";
        std::cout << "L: "<< design.L << ", ";
        std::cout << "drt: "<< design.drt << ", ";
        std::cout << "dst: "<< design.dst << ", ";
        std::cout << "Tphase: "<< design.Tphase << " ]\n"<< std::endl;
    }

    void print_performance(){
        std::cout<< "[ eff: "<< performance.efficiency << ", ";
        std::cout<< "T: "<< performance.torque << ", ";
        std::cout<< "I: "<< performance.current << ", ";
        std::cout<< "Cu_loss: "<< performance.copper_loss << ", ";
        std::cout<< "Iron_loss: "<< performance.iron_loss << ", ";
        std::cout<< "Total_loss: "<< performance.iron_loss + performance.copper_loss << " ]\n"<< std::endl;
    }

    void print_all(){
        std::cout<< "[ \nSRM VARIABLES >>";
        print_variables();
        std::cout<< "\nDEISGN OUTPUT>>";
        print_design();
        std::cout<< "\nPERFORMANCE ESTIMATION>>";
        print_performance();
        std::cout<< " ]\n";
    }

    void solve(struct parameters P){

    }

    void estimate_efficiency(struct eval_func EF){
        performance.efficiency = EF.W_efficiency*performance.efficiency + EF.W_cost*performance.cost;
    }

};
