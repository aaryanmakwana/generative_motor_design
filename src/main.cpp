#include "bldc.hpp"
#include <iostream>
#include <thread>

#define N_CHIDREN 10
#define N_GENERATION 100
#define THREAD_MAX 8
#define N_TOPPERS 3

void get_new_generation(bldc* BLDC);
bldc_params params = {50.00,1500.00,415.00,0.95};
fitness_function fitness_func = {15,1,10};

unsigned seed;

int main(){

    bldc BLDC[N_CHIDREN];
    srand(time(0));

    for (int i=0;i<N_CHIDREN;i+=1) {
        BLDC[i].init(params);
        BLDC[i].guess_variables();
    }

    std::thread threads[N_CHIDREN];

    for(int j=0;j<N_GENERATION;j+=1){
        std::cout << "Generation : " << j << "\n";
        //std::cout << "\tDesigning BLDC motors" << std::endl;
        for (int i=0;i<N_CHIDREN;i+=1) {
            design_bldc(&BLDC[i]);
        }
        //std::cout << "\tDesign complete" << std::endl;
        //std::cout << "\tEstimating performance" << std::endl;
        double eff_avg = 0;
        double eff_max = 0;
        for (int i=0;i<N_CHIDREN;i+=1) {
            estimate_performance(&BLDC[i]);
            eff_avg += BLDC[i].performance.efficiency;
            if (BLDC[i].performance.efficiency > eff_max) {
                eff_max = BLDC[i].performance.efficiency;
            }
        }
        eff_avg /= N_CHIDREN;
        std::cout << "\tAverage efficiency: " << eff_avg << "\n";
        std::cout << "\tMaximum efficiency: " << eff_max << "\n\n";
        //std::cout << "\tPerformance estimation complete" << std::endl;
        //std::cout << "\tSelecting top performers\n" << std::endl;
        get_new_generation(BLDC);
    }
    return 0;
}

void get_new_generation(bldc* BLDC){
    bldc* new_BLDC = new bldc[N_CHIDREN];
    for (int i=0;i<N_CHIDREN;i+=1) {
        new_BLDC[i].init(params);
        new_BLDC[i].guess_variables();
    }

    double fitness;
    double max_fitness[N_TOPPERS] = {0.0,0.0,0.0};
    int max_indexes[N_TOPPERS] = {-1,-1,-1};

    for (int i=0;i<N_CHIDREN;i+=1) {
        fitness = BLDC[i].fitness(fitness_func);
        if (fitness > max_fitness[0]) {
            max_fitness[2] = max_fitness[1];
            max_fitness[1] = max_fitness[0];
            max_fitness[0] = fitness;
            max_indexes[2] = max_indexes[1];
            max_indexes[1] = max_indexes[0];
            max_indexes[0] = i;
        } else if (fitness > max_fitness[1]) {
            max_fitness[2] = max_fitness[1];
            max_fitness[1] = fitness;
            max_indexes[2] = max_indexes[1];
            max_indexes[1] = i;
        } else if (fitness > max_fitness[2]) {
            max_fitness[2] = fitness;
            max_indexes[2] = i;
        }
    }

    new_BLDC[0] = breed(BLDC[max_indexes[0]], BLDC[max_indexes[1]]);
    new_BLDC[1] = breed(BLDC[max_indexes[1]], BLDC[max_indexes[2]]);
    new_BLDC[2] = breed(BLDC[max_indexes[2]], BLDC[max_indexes[0]]);
    new_BLDC[3] = BLDC[max_indexes[0]];
    new_BLDC[4] = BLDC[max_indexes[1]];
    new_BLDC[5] = BLDC[max_indexes[2]];

    for (int i=0;i<N_CHIDREN;i+=1) {
        BLDC[i] = new_BLDC[i];
    }

}
