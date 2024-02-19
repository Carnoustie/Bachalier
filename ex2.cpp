#include <gtkmm.h>
class MyDrawingArea : public Gtk::DrawingArea {
public:
    MyDrawingArea() {
        set_size_request(640, 480);
    }

protected:
    // Override the default signal handler for the 'draw' signal
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override {
        // Draw a line
        cr->move_to(10, 10); // Start point
        cr->line_to(200, 200); // End point
        cr->set_line_width(2.0); // Line width
        cr->stroke(); // Apply drawing

        return true;
    }
};

class MyAppWindow : public Gtk::Window {
public:
    MyAppWindow() {
        set_title("Line Drawing with GTKmm");
        set_default_size(640, 480);

        add(drawing_area); // Add the drawing area to the window

        show_all();
    }

private:
    MyDrawingArea drawing_area;
};

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create();

    MyAppWindow window;

    return app->run(window);
}

