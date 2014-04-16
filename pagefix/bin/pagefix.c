/*
	Page Fix
	Copyright (c) 2012 by Flyn - MIT license
	contains code from untouchable by baf (www.fabiszewski.net), with MIT license:
    http://www.opensource.org/licenses/mit-license.php
*/

#include <stdio.h> // printf
#include <stdbool.h> // bool
#include <string.h> // memset
#include <unistd.h> // usleep
#include <X11/XKBlib.h>

void pageForward(Display *display);
void SendMouseEvent(Display *display, int button, int x, int y, bool pressed);

bool debug = false;

int main(int argc, char * argv[]) {
	Window root;
	Display * display;
	XEvent xev;
	int screenWidth;
	int screenHeight;

	display = XOpenDisplay(NULL);
	if (display == NULL) {
		if (debug) printf("Could not open display\n");
		return 1;
	}

	root = DefaultRootWindow(display);
	screenWidth = WidthOfScreen(DefaultScreenOfDisplay(display));
	screenHeight = HeightOfScreen(DefaultScreenOfDisplay(display));
	if (debug) printf("Screen width: %i\n", screenWidth);
	if (debug) printf("Screen height: %i\n", screenHeight);

	XAllowEvents(display, AsyncBoth, CurrentTime);

	bool go = true;

	// We grab
	XGrabButton(display, AnyButton, AnyModifier, root, true, ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None);

	while (go) {
		XNextEvent(display, &xev);

		switch(xev.type) {
			case ButtonPress:
				if (debug) printf("Button press - %i\n", xev.xbutton.button);
				if (debug) printf("Coordinates - %i, %i\n", xev.xbutton.x, xev.xbutton.y);
			break;

			case ButtonRelease:
				if (debug) printf("Button release - %i\n", xev.xbutton.button);
				if (xev.xbutton.y>screenHeight-400)
					// If we touch on the bottom side of the screen, we go to the next page
					pageForward(display);
				else if (xev.xbutton.x>screenWidth-200 && xev.xbutton.y<200)
					// If we touch the upper right corner, we quit
					go = false;
				else
					// If touched anywhere else let's apply a simple touch where we are
					// It's not perfect, because it will disable gestures
					SendMouseEvent(display, Button1, xev.xbutton.x, xev.xbutton.y, true);
					usleep(10000);
					SendMouseEvent(display, Button1, xev.xbutton.x, xev.xbutton.y, false);
			break;
		}
	}

	XUngrabButton(display, AnyButton, AnyModifier, root);

	XCloseDisplay(display);

	return 0;
}

void pageForward(Display *display) {
	SendMouseEvent(display, Button1, 500, 300, true);
	usleep(10000);
	SendMouseEvent(display, Button1, 500, 300, false);
}

void pageBackward(Display *display) {
	SendMouseEvent(display, Button1, 50, 300, true);
	usleep(10000);
	SendMouseEvent(display, Button1, 50, 300, false);
}

void SendMouseEvent(Display *display, int button, int x, int y, bool pressed) {
	XEvent event;
	memset(&event, 0, sizeof(event));
	event.xbutton.type = pressed ? ButtonPress : ButtonRelease;
	event.xbutton.button = button;
	event.xbutton.same_screen = true;
	XQueryPointer(display, RootWindow(display, DefaultScreen(display)),
		&event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root,
		&event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

	event.xbutton.subwindow = event.xbutton.window;

	while (event.xbutton.subwindow) {
		event.xbutton.window = event.xbutton.subwindow;

		XQueryPointer(display, event.xbutton.window,
			&event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root,
			&event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
	}
	event.xbutton.x = x;
	event.xbutton.y = y;
	if (debug) printf("Sending %i %i event: %ix%i\n", event.xbutton.type, event.xbutton.button, x, y);

	if (!XSendEvent(display, PointerWindow, true, 0xfff, &event))
		printf("Failed to send mouse event\n");
	XFlush(display);
}
