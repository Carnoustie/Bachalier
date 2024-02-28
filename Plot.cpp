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
#include <Eigen/Dense>
#include <cstring>
//#include <eigen/Eigen/Sparse>

using namespace std;
using namespace Gtk;





    vector<coord> coordinateBuffer; //holds two-dimensional data points
    coord position; //current position for real-time plots 
    int lineWidth;

    Plot::Plot(){
        //title = "Please set title";
        X_axis_span = 1000.0;
        strcpy(title, "Please set title");
        numberOfBuffers = 0; //Initialize number of plots to zero
        set_size_request(640,480);  
        lineWidth = 1000;
        coordinateBuffer = vector<coord>();  //Allocate memory for coordinateBuffer
        //position.x = 400;
        //position.y = 1020;

        // Timer
        auto update_freq = 100; // Update frequency in milliseconds for real-time plots
       // Glib::signal_timeout().connect(sigc::mem_fun(*this, &Plot::on_timeout), update_freq);

    }

    /*arg1: A pointer to a coordinate buffer (i.e a pointer to a data series of x/y- coordinates)
    Return: Void
    Side effects: Appends the data series to the collection of data series held in "coordinateBufferPointers" */
    void Plot::addPlot(vector<coord> * dataPointer){
        coordinateBufferPointers[numberOfBuffers] = dataPointer;
        numberOfBuffers++;
    }


    /*Arg1: A single coordinate
    Return: Void
    Side Effects: For real time data, appends new data to the right end of the x-axis*/
    void Plot::updateBuffer(coord c){
        if(coordinateBuffer.size()<1000){
            coordinateBuffer.push_back(c);
        }else if(coordinateBuffer.size()>=1000){
            coordinateBuffer.erase(coordinateBuffer.begin());
            coordinateBuffer.push_back(c);
        }
    }


 
    /*Arg1: Number of desired bins
    /*Arg2: data series of coordinates
    Return: A Vector of coordinates ready for plotting the histogram
    Side Effects: None */
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
            
            while(currentVal>=currentFloor && currentVal<currentUpper){
                output[k].y+= 1.0;
                currentIndex++;
                currentVal = (double) data[currentIndex];
            }
        }
        return output;
    }

    /*Arg1: A pointer to an abstract Cairo-object that enables setting of plot graphics. See Cairo docs for more details
    Return: Void 
    Side Effects: Draws the x-y axes of the plotting window */
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

    /*Arg1: A pointer to an abstract Cairo-object that enables setting of plot graphics. See Cairo docs for more details
    Return: True on sucess, False on error 
    Side Effects: Draws the backround of the plotting window as well as the content of all the data buffers out to the plotting window*/
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
        contextPointer->show_text(title);
        contextPointer->move_to(xc+550, yc+80);
        contextPointer->show_text("time");
        contextPointer->set_line_width(2.0);
        contextPointer->set_source_rgb(107/255.0, 24/255.0, 24/255.0);
        contextPointer->move_to(xc-140,yc-950);
        contextPointer->set_font_size(44);
        contextPointer->show_text("$");
        contextPointer->set_font_size(24);

        for(int i=1; i<10; i++){
            contextPointer->move_to(xc-80,yc-i*100);
            contextPointer->show_text(to_string(i*10-20));
            contextPointer->move_to(xc-40,yc-i*100);
            contextPointer->line_to(xc, yc-i*100);

            contextPointer->move_to(xc+i*100,yc+40);
            contextPointer->show_text(to_string(i*10));
            contextPointer->move_to(xc+i*100,yc+20);
            contextPointer->line_to(xc+i*100, yc);


        }

        vector<coord> cb;
        for(int k = 0; k<numberOfBuffers;k++){
            
            cb = * coordinateBufferPointers[k];

            if(cb.size()>=1)
                for(int i=1; i< cb.size(); i++){
                    //cout << "\n\n" << "hit" << xc << "  " << yc <<"\n\n";
                    //cout << "\n\nxc and yc: " << xc << "  " << yc << "\n\n";
                   // contextPointer->move_to(xc+300,yc-300);
                   // contextPointer->line_to(xc+301, yc-301);
                   // cout << "\n\nfrom x:"<< xc + coordinateBuffer[i-1].x << "     from y: " << yc - coordinateBuffer[i-1].y << "\n\n"; 
                    //cout << "\n\nfrom x:"<< xc + cb[i-1].x << "     from y: " << yc - cb[i-1].y << "\n\n"; 
                    //cout << "\n\nto x:"<< xc + coordinateBuffer[i].x << "     to y: " << yc - coordinateBuffer[i].y << "\n\n"; 

                    contextPointer->move_to(xc +cb[i-1].x, yc-200-cb[i-1].y);
                    contextPointer->line_to(xc+ cb[i].x, yc-200 - cb[i].y);
                }
            contextPointer->stroke();

        }
        return true;
        
    }

    // For dynamic plotting. To-do 
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



