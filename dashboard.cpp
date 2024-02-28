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
#include <eigen/Eigen/Sparse>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>


using namespace std;
using namespace Gtk;


int main(int argc, char * argv[]){

    auto app = Application::create();
    Window w;
    w.set_default_size(2800,2360);
    w.move(2000,0);


    Plot p;
    w.add(p);
    w.show_all();
    w.set_title("Bloomburg");
    app->run(w);




}

