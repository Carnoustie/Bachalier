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


Eigen::VectorXd ItoProccess(int dim, double timeSpan, double timeStep, Eigen::VectorXd mu, Eigen::MatrixXd sigma);
Eigen::MatrixXd GaussianMatrix(int rows, int cols, double mu, double sigma);
Eigen::VectorXd GaussianVector(int dim, double mu, double sigma);
vector<coord> BrownianPath(double duration, double stepsize);
