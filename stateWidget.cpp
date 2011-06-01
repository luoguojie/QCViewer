#include "stateWidget.h"

StateWidget::StateWidget() : state (NULL) {}

bool StateWidget::on_expose_event(GdkEventExpose* event) {
  (void)event; // placate compiler..
  Glib::RefPtr<Gdk::Window> window = get_window();
  if(window) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
    cr->rectangle (0, 0, width, height);
    cr->set_source_rgb (1,1,1);
    cr->fill();
    draw_state (cr, state,(float)width,(float)height);
  }
  return true;
}
	
void StateWidget::set_state(State* n_state){
	state = n_state;
}