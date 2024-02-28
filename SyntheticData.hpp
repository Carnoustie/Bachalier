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
#include <eigen/Eigen/Dense>



using namespace std;
using namespace Gtk;


Eigen::VectorXd ItoProccess(int dim, double timeSpan, double timeStep, Eigen::VectorXd mu, Eigen::MatrixXd sigma);
Eigen::MatrixXd GaussianMatrix(int rows, int cols, double mu, double sigma);
Eigen::VectorXd GaussianVector(int dim, double mu, double sigma);
vector<coord> BrownianPath(double duration, double stepsize);
vector<coord> GeometricBrownianPath(double duration, double stepsize, double mu, double sigma, double IPO_price);
