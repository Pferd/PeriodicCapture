// Authored by someone: adopted and modified by Pferd (RC).

#include <gtk/gtk.h>

void displayUI(){

// UI Functionality here!

  GtkWidget *window;  // GTK widgets here
  GtkWidget *frame;
  GtkWidget *start;
  GtkWidget *stop;

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL); // Top level Window here!
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 1000,600);
  gtk_window_set_title(GTK_WINDOW(window), "screen capture");

  frame = gtk_fixed_new(); // Add frame to the container
  gtk_container_add(GTK_CONTAINER(window), frame);

  start = gtk_button_new_with_label("start"); // Button with labels, position and dimensions
  gtk_widget_set_size_request(start, 80, 35);
  gtk_fixed_put(GTK_FIXED(frame), start, 50, 20);

  stop = gtk_button_new_with_label("stop"); // same here
  gtk_widget_set_size_request(stop, 80, 35);
  gtk_fixed_put(GTK_FIXED(frame), stop, 150, 20);

  gtk_widget_show_all(window); // show them all

  g_signal_connect(window, "destroy", // call back function to quit.
      G_CALLBACK (gtk_main_quit), NULL);

// Some call back functions to be provided for the button events
// eventually the srcreen capture methods.
/*
  g_signal_connect(start, "clicked", 
      G_CALLBACK(increase), label);

  g_signal_connect(stop, "clicked", 
      G_CALLBACK(decrease), label);
*/

}

int main(int argc, char** argv) {

  gtk_init(&argc, &argv); // Initialize GTK
  displayUI(); // call display function
  gtk_main(); // Main loop to listen for events.

  return 0;
}
