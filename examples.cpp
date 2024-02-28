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
#include "SyntheticData.hpp"
#include <Eigen/Dense>
//#include <eigen/Eigen/Sparse>

using namespace std;
using namespace Gtk;


int main(int argc, char * argv []){

    Eigen::VectorXd gv = GaussianVector(5, 2, 5); //Example Gaussian Vector
    cout << "\n\n\n\n\n\nExample Gaussian Vector:\n\n" << gv;
    Eigen::MatrixXd gm = GaussianMatrix(5,5, 0, 1);
    cout << "\n\n\n\n\n\nExample Gaussian Matrix:\n\n" << gm;

    Eigen::MatrixXd cov = (gm + gm.transpose())/2; //example for covariance matrix to test other features
    Eigen::LLT<Eigen::MatrixXd> covLLT = cov.llt(); // LU-decomposition
    Eigen::MatrixXd covRoot = covLLT.matrixL(); // Std.dev in matrix-form

    Eigen::VectorXd mu = Eigen::VectorXd::Zero(5); //Zero matrix for example mean vector

    Eigen::VectorXd samplePrices = ItoProccess(5, 800, 1, mu, covRoot);  //Example endpoint prices with multidimensional intercorrelated Ito stocks.
    cout << "\n\n\n\n\n\nExample end prices with multidimensional Ito model:\n\n" << samplePrices << "\n\n";
    
    vector<coord> exampleBrownianPath = BrownianPath(800.0,0.1); //Generate Example Brownian Motion for plotting.
    auto app = Application::create();
    Window w;
    w.set_default_size(1600,1360);
    w.move(2000,0);
    Plot p;
    
    p.addPlot(&exampleBrownianPath);
    
    w.add(p);
    w.show_all();
    w.set_title("Example brownian motion");
    strcpy(p.title, "Example brownian motion");
    app->run(w);
}
