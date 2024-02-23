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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <math.h>

using namespace std;
using namespace Gtk;

random_device rd{}; //True number generator from hardware noise
mt19937 gen{rd()}; //Mersenne twister to output Uniform distribution given random integers from rd.
normal_distribution<> nd{0.0, 1.0}; //Gaussian number generator from Inverse Sampling using Uniform distribution




int main(int argc, char * argv[]){
    auto app = Application::create();
    Window w;
    w.set_default_size(1600,1360);
    w.move(2000,0);
    Plot p;
    float sigma = 3;
    vector<double> samples;
    for(int k =0; k<100000;k++){
       double gaussian = sigma*nd(gen);
       samples.push_back(gaussian);
      // cout << "\n\nsample: " << samples[k];
    }
    //cout << "\n\n\n\n\n\nbreak\n\n\n\n\n\n";
    sort(samples.begin(),samples.end());
    vector<coord> histExample = p.histogram(700,samples);
    //p.addPlot(histExample);
        

    vector<coord> sineBuffer;

    for(int k=0;k<1000;k++){
        coord c = { (double) k, 100*sin(0.04*k)};
        sineBuffer.push_back(c);
    }

    vector<coord> * bp = &sineBuffer;

    vector<coord> newValue = *bp;
    vector<coord> * hp = &histExample;
    p.coordinateBuffer = sineBuffer;
    p.addPlot(bp);
    p.addPlot(hp);
    //p.addPlot(sineBffer);


    //coord c1 = {0,300};
    //coord c2 = {200,550};
    //p.updateBuffer(c1);
    //p.updateBuffer(c2);
    w.add(p);
    w.show_all();
    w.set_title("Asset Price Dynamics");
    app->run(w);


    return 1;

}
