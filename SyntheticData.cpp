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
vector<coord> examplePath(800);


//Independent Increments
//

Eigen::VectorXd GaussianVector(int dim, double mu, double sigma){
    Eigen::VectorXd gv(dim);
    for(int k=0; k<dim; k++){
        gv(k) = mu +  sigma*nd(gen);
    }
    return gv;
}


Eigen::VectorXd ItoProccess(int dim, double timeSpan, double timeStep, Eigen::VectorXd mu, Eigen::MatrixXd sigma){
    //double paths[dim];
    

    int N = timeSpan/timeStep;
    Eigen::VectorXd priorY = Eigen::VectorXd::Zero(dim);
    Eigen::VectorXd nextY;

    Eigen::VectorXd priorX = Eigen::VectorXd::Zero(dim);
    Eigen::VectorXd nextX;
    Eigen::VectorXd stepX = timeStep*Eigen::VectorXd::Ones(dim);
    
    Eigen::VectorXd gaussianIncrement;
    double rootStep = sqrt(timeStep);
    
    cout << "\n\nN: " << N << "\n\n";
    
    coord forTest;

    for(int k=0; k<N; k++){



        //cout << "hej";

        nextX = priorX + stepX;
        priorX = nextX;
    
        gaussianIncrement = GaussianVector(dim, 0, 1);
        nextY = priorY + timeStep*mu + rootStep*sigma*gaussianIncrement;
        priorY = nextY;
        
        forTest.x = nextX(4);
        forTest.y = nextY(4);

        cout <<"\n\nx: " << forTest.x  << "\n\n"; 
        cout <<"\n\ny: " <<  forTest.y <<"\n\n"; 
        examplePath[k] = forTest;


        //currentPoint.x = nextX;
        //currentPoint.y = nextY;

        //paths[k] = currentPoint;
    }
    



    return nextY;
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
    Eigen::MatrixXd gm = GaussianMatrix(5,5, 0, 1);
    Eigen::MatrixXd cov = (gm + gm.transpose())/2; //example for covariance matrix to test other features
    cout <<"\n\n" <<  cov << "\n\n";
    Eigen::LLT<Eigen::MatrixXd> covLLT = cov.llt();
    Eigen::MatrixXd covRoot = covLLT.matrixL();

    Eigen::VectorXd mu = Eigen::VectorXd::Zero(5);

    Eigen::VectorXd samplePrices = ItoProccess(5, 800, 1, mu, covRoot);
    cout << "\n\n" << samplePrices << "\n\n";



    auto app = Application::create();
    Window w;
    w.set_default_size(1600,1360);
    w.move(2000,0);
    Plot p;
    
    p.addPlot(&examplePath);
    
    w.add(p);
    w.show_all();
    w.set_title("Asset Price Dynamics");
    app->run(w);


}
