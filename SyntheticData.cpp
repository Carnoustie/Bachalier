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


double** ItoProccess(int dim, double timeSpan, double timeStep, Eigen::VectorXd mu, Eigen::MatrixXd sigma){
    double** paths;
    

    int N = timeSpan/timeStep;
    
    double priorY = 0.0;
    double nextY;

    double priorX = 0.0;
    double nextX;
    
    Eigen::VectorXd gaussianIncrement;
    double rootStep = sqrt(timeStep);
    Eigen::VectorXd gaussianIncrement;
    for(int k=0; k<N; k++){
        nextX = priorX + timeStep;
        priorX = nextX;

        gaussianIncrement = GaussianVector(dim, 0, 1);
        nextY = priorY + timeStep*mu + rootStep*sigma*gaussianIncrement;
        priorY = nextY;


        currentPoint.x = nextX;
        currentPoint.y = nextY;

        path[k] = currentPoint;
    }

    return path;
}




//Independent Increments
//

Eigen::VectorXd GaussianVector(int dim, double mu, double sigma){
    Eigen::VectorXd gv(dim);
    for(int k=0; k<dim; k++){
        gv(k) = mu +  sigma*nd(gen);
    }
    return gv;
}

Eigen::MatrixXd GaussianMatrix(int rows, int cols, double mu, double sigma){
    Eigen::MatrixXd gm(rows, cols);
    for(int k=0; k<rows; k++){
        for(int i=0; i<cols; i++){
            gm(k,i)= mu + sigma*nd(gen);
        }
    }
    return gm;
}



int main(int argc, char * argv[]){

    Eigen::VectorXd gv = GaussianVector(5, 2, 5);
//    cout <<"\n\n" <<  gv << "\n\n";
    Eigen::MatrixXd gm = GaussianMatrix(3,3, 0, 1);
    Eigen::MatrixXd cov = (gm + gm.transpose())/2; //example for covariance matrix to test other features
    cout <<"\n\n" <<  cov << "\n\n";
    Eigen::LLT<Eigen::MatrixXd> covLLT = cov.llt();
    Eigen::MatrixXd covRoot = covLLT.matrixL();

}
