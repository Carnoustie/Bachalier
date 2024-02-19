#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <random>
#include <iostream>

using namespace std;

int main(int argc, char * argv[]){

    cout << "\n\nWelcome to the financial market!\n\n";
    
    random_device rd{}; //True number generator from hardware noise
    mt19937 gen{rd()}; //Mersenne twister to output Uniform distribution given random integers from rd.

    normal_distribution<> nd{0.0, 100}; //Gaussian number generator from Inverse Sampling using Uniform distribution

    double gaussian = nd(gen);

    cout << "\n\nGaussian sample:\n\n" << gaussian <<"\n\n";

}


