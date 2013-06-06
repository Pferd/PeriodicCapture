#include <pthread.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>  
#include <unistd.h> 


int main(int argc, char** argv) {
// Logging starts here!
printf("-----------------------------------------------------------------\n");
printf("Main function started!\n");
// function pointer to capture screen
    Display *disp;
    Window root;
    cairo_surface_t *surface;
    cairo_status_t status; // to get the error status.
    const char *PNGError; // to get the string representation of error.
    int scr;
    
    pthread_t writeThread; // A thread to write the data to a file.
    int threadStatus; // Integer to get the thread status error messages 
    int processID;
    unsigned int mainthreadID;

mainthreadID=(unsigned int)pthread_self(); 
printf("The Main thread id is: %u\n",mainthreadID);

// Function pointer that write the data to a file

void *writeToFile(void *PSurface){

processID=getpid();
unsigned int threadID=(unsigned int)pthread_self();

printf("Main Thread suspended......\n"); 
printf("PID of this process: %d\n",processID); 
printf("The ID of this thread is: %u\n",threadID);
printf("-----------------------------------------------------------------\n"); 
cairo_surface_t *DispSurface;
DispSurface = PSurface;

status=cairo_surface_write_to_png(DispSurface,"test.png"); 
if(!status){
	printf("No problems in writing to File!\n");
}
else {
printf("The error status number is: %d \n",status);
}

PNGError=cairo_status_to_string(status); 
printf("Cairo API says: %s\n",PNGError);
printf("-----------------------------------------------------------------\n"); 
cairo_surface_destroy(surface); 
printf("Memory freed! \n"); 
printf("-----------------------------------------------------------------\n"); 
}// End of write function!

printf("-----------------------------------------------------------------\n");
printf("Variables initialized and trying to connect to Display\n"); 
    disp = XOpenDisplay(NULL);
    scr = DefaultScreen(disp);
printf("connected to default screen\n");
    root = DefaultRootWindow(disp);

printf("getting the screen shot from the display\n");
printf("-----------------------------------------------------------------\n");
    surface = cairo_xlib_surface_create(disp, root, DefaultVisual(disp, scr),
            DisplayWidth(disp, scr), DisplayHeight(disp, scr));

// This function will write the data to a png file.
printf("successfully done!! Now trying to write the content to a png file\n");
printf("-----------------------------------------------------------------\n");

threadStatus=pthread_create(&writeThread,NULL,writeToFile,(void *)surface);

// Display error for threads!
if(threadStatus){
printf("Unable to create thread!");
return 1;
}

pthread_join(writeThread,NULL);
mainthreadID=(unsigned int)pthread_self();
printf("Joining back to Main thread Again! Thread ID is %u\n",mainthreadID);
printf("-----------------------------------------------------------------\n");

return 0; // End of Main function
}
