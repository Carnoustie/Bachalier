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


using namespace std;
using namespace Gtk;

struct coord{
    double x;
    double y;
};


class Plot : public Gtk::DrawingArea{

public:

    Plot(); //Constructor
    
    void updateBuffer(coord c);
    void addPlot(vector<coord> * dataPointer);

    vector<coord> histogram(int N, vector<double> data);

    //vector<double> sample_Path(int N_steps, )

    vector<coord> coordinateBuffer;
    


    int numberOfBuffers;
    
    coord position;
    int lineWidth;
  
    vector<coord> * coordinateBufferPointers[100];
    
    double X_axis_span;
    char title[100];
private:
    void drawAxes(const Cairo::RefPtr<Cairo::Context>& contextPointer);
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& contextPointer) override;
    bool on_timeout();
};
