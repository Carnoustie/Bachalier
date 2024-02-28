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

//#include "Plot.hpp"
#include <Eigen/Dense>
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

struct coord{
    double x;
    double y;
};


//Independent Increments
//


vector<coord> GeometricBrownianPath(double duration, double stepsize, double mu, double sigma, double IPO_price){
    int N_steps = round(duration/stepsize);
    vector<coord> output(N_steps);
    
    double prevY = IPO_price;
    double currY = IPO_price;

    double prevX = 0.0;
    double currX = 0.0;

    cout << "\n\nFactor: " << currY;
    double dy;
    double dx;
    double rootStep = sqrt(stepsize);
    for(int k=0; k<N_steps; k++){

        double factor = exp( (mu-sigma*sigma/2)*stepsize + sigma* rootStep*nd(gen));

        cout << "\n\ncurrY: " << prevY << "         factor: " << factor;
        currY = (double) prevY*factor;
        currX = prevX + stepsize;
        prevY = currY;
        prevX = currX; 
        output[k].y = currY;
        output[k].x = currX;
    }

    for(int k=0; k<N_steps; k++){
//        cout << "\n\n\n\ny = " << path[k].y << "\n\n";
  //      cout << "\n\nx = " << path[k].x << "\n\n";
    }


    return output;
}



vector<coord> BrownianPath(double duration, double stepsize){
    int N_steps = round(duration/stepsize);
    vector<coord> path(N_steps);
    
    double priorY = 0.0;
    double currentY = 0.0;

    double priorX = 0.0;
    double currentX = 0.0;

    double dy;
    double dx;
    double sigma = sqrt(stepsize);
    for(int k=0; k<N_steps; k++){
        path[k].y = currentY;
        path[k].x = currentX;
        double dy = sigma*nd(gen); //Gaussian step with brownian property sigma = root(dt)
        currentY = priorY + dy;
        currentX = priorX + stepsize;
        priorY = currentY;
        priorX = currentX;
    }
    return path;
}

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

        //examplePath[k] = forTest;


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


/*
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
*/
