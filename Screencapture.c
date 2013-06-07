// Authored by someone: adopted and modified by Pferd (RC).

#include <gtk/gtk.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <pthread.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

// variables to be used to catch status and errors.
static cairo_status_t status;	// to get the error status.
static const char *PNGError;	// to get the string representation of error.  



void *writeToFile(void *PSurface)
{

    printf("In Write thread, with ID: %u\n",(unsigned int) pthread_self());
    cairo_surface_t *DispSurface;
    DispSurface = PSurface;

    status = cairo_surface_write_to_png(DispSurface, "test.png");
    if (!status) {
	printf("No problems in writing to File!\n");
    } else {
	printf("The error status number is: %d\n", status);
    }

    PNGError = cairo_status_to_string(status);
    printf("Cairo API says: %s\n", PNGError);
    printf("-----------------------------------------------------------------\n");
    cairo_surface_destroy(DispSurface);
    printf("Memory freed!\n");
    printf("-----------------------------------------------------------------\n");

}// End of write function and Thread!


void Dshow(GtkWidget * widget, gpointer drawArea)
{

    GdkPixbuf *pix;
    GError *err = NULL;

    printf("we are inside the show");
    /* Create pixbuf */
    pix = gdk_pixbuf_new_from_file("/home/ramu/Desktop/PeriodicCapture/test.png", &err);
    pix = gdk_pixbuf_scale_simple(pix, 850, 600, GDK_INTERP_BILINEAR);
    if (err) {
	printf("Error : %sn", err->message);
	g_error_free(err);
	return;
    }

    GdkDrawable *draw = gtk_widget_get_window(drawArea);
    /* Draw pixbuf */
    gdk_draw_pixbuf(draw, NULL, pix, 0, 0, 0, 0, -1, -1, GDK_RGB_DITHER_NONE, 0, 0);
    return;
}

// Capture Function starts here!
void Scapture(GtkWidget * widget, gpointer drawArea)
{

    Display *disp;
    Window root;
    cairo_surface_t *surface;
    int scr;

    pthread_t writeThread;// Thread to write the capture screen.

    disp = XOpenDisplay(NULL);
    scr = DefaultScreen(disp);
    root = DefaultRootWindow(disp);
    surface = cairo_xlib_surface_create(disp, root, DefaultVisual(disp, scr), DisplayWidth(disp, scr), DisplayHeight(disp, scr));

// Display error for threads!
    if (pthread_create(&writeThread, NULL, writeToFile, (void *)surface)) {
	printf("Unable to create thread!\n");
	exit(1);
    }
}// End of capture fucntion

void *displayUI()
{

// UI Functionality starts here!

    printf("UI thread started here with id: %u \n", (unsigned int) pthread_self());

    GtkWidget *window;		// GTK Widgets pointers
    GtkWidget *frame;
    GtkWidget *capture;
    GtkWidget *show;
    GtkWidget *label;
    GtkWidget *drawArea;
    drawArea = gtk_drawing_area_new();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);	// Top level Window here!
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_NONE);// window with center position
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 650);	// size of the window
    gtk_window_set_title(GTK_WINDOW(window), "screen capture");	// Title to the window

    frame = gtk_fixed_new();	// get a new frame/box to hold widgets
    gtk_container_add(GTK_CONTAINER(window), frame);// add the frame to the window

// Apparently, widgets are hooked to the frame.

    gtk_widget_set_size_request(drawArea, 850, 800);	// sets the size
    gtk_fixed_put(GTK_FIXED(frame), drawArea, 50, 80);	// and the location on frame.
    //g_signal_connect (G_OBJECT(drawArea), "draw", G_CALLBACK(draw_callback), NULL); // call-back to drawing area.
    //g_signal_connect(drawArea,"expose-event",G_CALLBACK(da_expose),NULL);

    capture = gtk_button_new_with_label("capture");	// Button with labels, position and dimensions
    gtk_widget_set_size_request(capture, 80, 35);	// sets the button size
    gtk_fixed_put(GTK_FIXED(frame), capture, 50, 20);	// and the location on frame.

    show = gtk_button_new_with_label("show");	// same here
    gtk_widget_set_size_request(show, 80, 35);	// -do-
    gtk_fixed_put(GTK_FIXED(frame), show, 150, 20); // -do-

    label = gtk_label_new("Logs Here!");
    gtk_fixed_put(GTK_FIXED(frame), label, 950, 100);

    gtk_widget_show_all(window);        //show them all

    g_signal_connect(window, "destroy",	// call back function to quit.
		     G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(capture, "clicked", G_CALLBACK(Scapture), drawArea);

    g_signal_connect(show, "clicked", G_CALLBACK(Dshow), drawArea);

}

// Start of Main thread

int main(int argc, char **argv)
{

    printf("Main thread started here with id: %u\n", (unsigned int) pthread_self());

    gtk_init(&argc, &argv);	// Initialize GTK

    pthread_t dpThread;		// Thread for UI

    if (pthread_create(&dpThread, NULL, displayUI, NULL)) {
	printf("Unable to create User Interface Thread");
    }

    gtk_main();	// Main loop to listen for events.
    return 0;
}

