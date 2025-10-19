#include "window.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <X11/Xutil.h>

using namespace std;

Xwindow::Xwindow(int width, int height) : width{width}, height{height} {
    d = XOpenDisplay(NULL);
    if (d == NULL) {
        cerr << "Cannot open display" << endl;
        exit(1);
    }
    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1, BlackPixel(d, s), WhitePixel(d, s));
    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapRaised(d, w);

    Pixmap pix = XCreatePixmap(d, w, width, height, DefaultDepth(d, DefaultScreen(d)));
    gc = XCreateGC(d, pix, 0, (XGCValues *)0);

    XFlush(d);
    XFlush(d);

    // Set up colours.
    XColor xcolour;
    Colormap cmap;
    cmap = DefaultColormap(d, DefaultScreen(d));
    
    // Basic colors
    char color_vals[5][10] = {"white", "black", "red", "green", "blue"};
    for (int i = 0; i < 5; ++i) {
        XAllocNamedColor(d, cmap, color_vals[i], &xcolour, &xcolour);
        colours[i] = xcolour.pixel;
    }

    // Additional colors using RGB values.
    xcolour.red = 177 * 256; xcolour.green = 110 * 256; xcolour.blue = 65 * 256; xcolour.flags = DoRed | DoGreen | DoBlue;
    XAllocColor(d, cmap, &xcolour);
    colours[5] = xcolour.pixel;

    xcolour.red = 254 * 256; xcolour.green = 213 * 256; xcolour.blue = 153 * 256; xcolour.flags = DoRed | DoGreen | DoBlue;
    XAllocColor(d, cmap, &xcolour);
    colours[6] = xcolour.pixel;

    XSetForeground(d, gc, colours[1]);

    // Make window non-resizeable.
    XSizeHints hints;
    hints.flags = (USPosition | PSize | PMinSize | PMaxSize);
    hints.height = hints.base_height = hints.min_height = hints.max_height = height;
    hints.width = hints.base_width = hints.min_width = hints.max_width = width;
    XSetNormalHints(d, w, &hints);

    XSynchronize(d, True);

    usleep(1000);
}

Xwindow::~Xwindow() {
    XFreeGC(d, gc);
    XCloseDisplay(d);
}

int Xwindow::getWidth() const { return width; }
int Xwindow::getHeight() const { return height; }

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
    XSetForeground(d, gc, colours[colour]);
    XFillRectangle(d, w, gc, x, y, width, height);
    XSetForeground(d, gc, colours[1]);
}

void Xwindow::drawString(int x, int y, string msg, int colour) {
  XSetForeground(d, gc, colours[colour]);
  Font f = XLoadFont(d, "6x13");
  XTextItem ti;
  ti.chars = const_cast<char*>(msg.c_str());
  ti.nchars = msg.length();
  ti.delta = 0;
  ti.font = f;
  XDrawText(d, w, gc, x, y, &ti, 1);
  XSetForeground(d, gc, colours[Black]);
  XFlush(d);
}
