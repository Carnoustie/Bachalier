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


using namespace std;
using namespace Gtk;

/*
random_device rd{}; //True number generator from hardware noise
mt19937 gen{rd()}; //Mersenne twister to output Uniform distribution given random integers from rd.
normal_distribution<> nd{0.0, 1.0}; //Gaussian number generator from Inverse Sampling using Uniform distribution
*/





    vector<coord> coordinateBuffer;
    coord position;
    int lineWidth;

    Plot::Plot(){
        set_size_request(640,480); 
        lineWidth = 1000;
        coordinateBuffer = vector<coord>();
        //position.x = 400;
        //position.y = 1020;

        // Timer
        auto update_freq = 100; // Update frequency in milliseconds
       // Glib::signal_timeout().connect(sigc::mem_fun(*this, &Plot::on_timeout), update_freq);

    }

    void Plot::addPlot(vector<coord> data){
        coordinateBuffer = data;
    }

    void Plot::updateBuffer(coord c){
        if(coordinateBuffer.size()<1000){
            coordinateBuffer.push_back(c);
        }else if(coordinateBuffer.size()>=1000){
            coordinateBuffer.erase(coordinateBuffer.begin());
            coordinateBuffer.push_back(c);
        }
    }


    vector<coord> Plot::histogram(int N, vector<double> data){
        sort(data.begin(), data.end());
        double valueSpan =  data.back() - data.front();
        for(int k = 0; k<data.size(); k++){
           // cout << "\n\ndata[" <<k<< "] : " << data[k];
        }
        double h =  valueSpan/N;
        double bottom = data[0];
        double currentVal = bottom;
        double nextVal = data[1];
        vector<coord> output(N);
        int currentIndex = 0;
        double currentFloor, currentUpper;

        for(int k=0; k<N; k++){
            output[k].x = (double) k;
            output[k].y = 0.0;
            currentFloor = (double) bottom + k*h;
            currentUpper = (double) currentFloor + h;
            
            //cout << "\n\nlower : " << currentFloor << " upper: " << currentUpper << "\n\n";
           //cout << "\n\nPriorval: : " << output[k].y;



            while(currentVal>=currentFloor && currentVal<currentUpper){
             //   cout << "\n\ncurrentVal: " << currentVal;
                output[k].y+= 1.0;
                currentIndex++;
                currentVal = (double) data[currentIndex];
            }
           // cout << "\n\ncounts for " << k << " : " << output[k].y << "\n\n";
        }
        return output;
    }


    /*
    vector<double> sample_Path(int N_steps, double, mu, double sigma){

    }
*/ 

    
    void Plot::drawAxes(const Cairo::RefPtr<Cairo::Context>& contextPointer){
        Allocation allocation = get_allocation();
        const int width = allocation.get_width();
        const int height = allocation.get_height();
        int xc, yc;
        xc = width/4;
        yc = 3*height/4;
        contextPointer->set_line_width(10.0);
        contextPointer->set_source_rgb(240/255.0, 20/255.0, 111/255.0);
        contextPointer->move_to(xc,yc);
        contextPointer->line_to(xc+1000, yc);
        contextPointer->move_to(xc, yc+5);
        contextPointer->line_to(xc, yc-1000);
        contextPointer->stroke();



    }


    bool Plot::on_draw(const Cairo::RefPtr<Cairo::Context>& contextPointer) {
        Allocation allocation = get_allocation();
        const int width = allocation.get_width();

        const int height = allocation.get_height();
        Cairo::RefPtr<Cairo::LinearGradient> gradient = Cairo::LinearGradient::create(0, 0, 0, height);
        gradient->add_color_stop_rgba(0, 0.85, 0.65, 0.6, 1); // Light blue at the top
        gradient->add_color_stop_rgba(1, 0.9, 0.9, 0.90, 1); // White at the bottom
        contextPointer->set_source(gradient);
        contextPointer->rectangle(0, 0, width, height);
        contextPointer->fill();
        //drawAxes(contextPointer);

        //cout << "\n\n"<< coordinateBuffer.size() << "\n\n";
        int xc, yc;
        xc = width/4;
        yc = 3*height/4;
        contextPointer->set_line_width(6.0);
        contextPointer->set_source_rgb(32/255.0, 29/255.0, 71/255.0);
        contextPointer->move_to(xc,yc);
        contextPointer->line_to(xc+1000, yc);
        contextPointer->move_to(xc, yc+3);
        contextPointer->line_to(xc, yc-1000);
        contextPointer->stroke();
        
        contextPointer->select_font_face("Playfair Display", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
        contextPointer->set_font_size(24);
        contextPointer->move_to(xc+350,yc-960);
        contextPointer->show_text("Gaussian Histogram");
        contextPointer->move_to(xc+550, yc+40);
        contextPointer->show_text("");
        contextPointer->set_line_width(2.0);
        contextPointer->set_source_rgb(107/255.0, 24/255.0, 24/255.0);
        contextPointer->move_to(xc-140,yc-950);
        contextPointer->set_font_size(44);
        contextPointer->show_text("$");
        contextPointer->set_font_size(24);

        for(int i=1; i<10; i++){
            contextPointer->move_to(xc-80,yc-i*100);
            contextPointer->show_text(to_string(i*10));
            contextPointer->move_to(xc-40,yc-i*100);
            contextPointer->line_to(xc, yc-i*100);


        }

        if(coordinateBuffer.size()>=1)
            for(int i=1; i< coordinateBuffer.size(); i++){
                //cout << "\n\n" << "hit" << xc << "  " << yc <<"\n\n";
                //cout << "\n\nxc and yc: " << xc << "  " << yc << "\n\n";
               // contextPointer->move_to(xc+300,yc-300);
               // contextPointer->line_to(xc+301, yc-301);
                //cout << "\n\nfrom x:"<< xc + coordinateBuffer[i-1].x << "     from y: " << yc - coordinateBuffer[i-1].y << "\n\n"; 
                //cout << "\n\nto x:"<< xc + coordinateBuffer[i].x << "     to y: " << yc - coordinateBuffer[i].y << "\n\n"; 

                contextPointer->move_to(xc+200 +coordinateBuffer[i-1].x, yc-200-coordinateBuffer[i-1].y);
                contextPointer->line_to(xc+ +200+ coordinateBuffer[i].x, yc-200 - coordinateBuffer[i].y);
            }
        contextPointer->stroke();


        return true;
    }

    bool Plot::on_timeout(){
       /* float sigma = 3.0;
        double gaussian = sigma*nd(gen);
        if(coordinateBuffer.size()<1000)
            position.x+=1;

        position.y+=gaussian;
        updateBuffer(position);
        //cout << "\n\nGaussian sample:\n\n" << gaussian <<"\n\n";
        queue_draw();*/
        return true;
    }



