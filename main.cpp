#include "bldc.hpp"

#define N_CHIDREN 10
#define N_GENERATION 100

int main(){
    bldc_params params = {50.00,1500.00,415.00};

    bldc BLDC[N_CHIDREN];
    for (int i=0;i<N_CHIDREN;i+=1) {
        srand(time(0));
        BLDC[i].init(params);
        BLDC[i].guess_variables();
    }
    return 0;
}
