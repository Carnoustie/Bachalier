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
#include <Eigen/Dense>


using namespace std;
using namespace Gtk;


random_device rd{}; //True number generator from hardware noise
mt19937 gen{rd()}; //Mersenne twister to output Uniform distribution given random integers from rd.
normal_distribution<> nd{0.0, 1.0}; //Gaussian number generator from Inverse Sampling using Uniform distribution

struct coord{
    double x;
    double y;
};




//Arg 1: Length of vector 
//Arg 2: mean of each element
//Arg 3: std. dev of each element
//Return: Gaussian Vector with desired properties
Eigen::VectorXd GaussianVector(int dim, double mu, double sigma){
    Eigen::VectorXd gv(dim);
    for(int k=0; k<dim; k++){
        gv(k) = mu +  sigma*nd(gen);
    }
    return gv;
}




//Arg 1: Rows
//Arg 2: columns
//Arg 2: mean of each element
//Arg 3: std. dev of each element
//Return: Gaussian Matrix with desired properties
Eigen::MatrixXd GaussianMatrix(int rows, int cols, double mu, double sigma){
    Eigen::MatrixXd gm(rows, cols);
    for(int k=0; k<rows; k++){
        for(int i=0; i<cols; i++){
            gm(k,i)= mu + sigma*nd(gen);
        }
    }
    return gm;
}






//Arg 1: Duration of brownian motion dynamics
//Arg 2: size of time step
//Arg 3: drift
//Arg 4: diffusion
//Arg 5:Initial Value
//Return: points along the taken path in a vector of coordinate
vector<coord> GeometricBrownianPath(double duration, double stepsize, double mu, double sigma, double IPO_price){
    int N_steps = round(duration/stepsize);
    vector<coord> output(N_steps);
    
    double priorY = IPO_price;
    double currentY = IPO_price;

    double priorX = 0.0;
    double currentX = 0.0;

    double dy;
    double dx;
    double rootStep = sqrt(stepsize);
    for(int k=0; k<N_steps; k++){

        double factor = exp( (mu-sigma*sigma/2)*stepsize + sigma* rootStep*nd(gen));

        currentY= priorY*factor;
        currentX = priorX + stepsize;
        priorY = currentY;
        priorX = currentX; 
        output[k].y = currentY;
        output[k].x = currentX;
    }

    return output;
}


//Arg 1: Duration of brownian motion dynamics
//Arg 2: size of time step
//Return: points along the taken path in a vector of coordinate
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




//Arg 1: Length of vector 
//Arg 2: Duration for Ito Proccess path
//Arg 3: time step size
//Arg 4: Drift
//Arg 5: Diffusion
//Return: Vector of end point values of the standard process (working on storing the entire path history in a suitable data structure). 
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




