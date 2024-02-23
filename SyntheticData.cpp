#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <gtkmm.h>
#include <cairomm/context.h>
#include <vector>
#include <random>
#include <algorithm>

#include "Plot.hpp"
#include <eigen/Eigen/Dense>
//#include <eigen/Eigen/Sparse>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>


using namespace std;
using namespace Gtk;


random_device rd{}; //True number generator from hardware noise
mt19937 gen{rd()}; //Mersenne twister to output Uniform distribution given random integers from rd.
normal_distribution<> nd{0.0, 1.0}; //Gaussian number generator from Inverse Sampling using Uniform distribution

/*
vector<coord>* ItoProccess(int dim, double timeSpan, double stepSize){

}
*/



//Independent Increments
//

Eigen::VectorXd GaussianVector(int dim, double mu, double sigma){
    Eigen::VectorXd gv(dim);
    for(int k=0; k<dim; k++){
        gv(k) = mu +  sigma*nd(gen);
    }
    return gv;
}



int main(int argc, char * argv[]){

    Eigen::VectorXd gv = GaussianVector(5, 2, 5);
    cout <<"\n\n" <<  gv << "\n\n";
}
