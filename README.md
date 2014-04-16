kindle-pagefix
==============

Small KUAL extension to allow easier reading with the left hand

Once launched, any touch on the lower half off the screen will be transformed into a touch on the 500x300 coordinates. (on the right side, activating a page flip)

Touching anywhere else will raise a *simple* (no dragging etc.) touch event where you actually touched the screen.

Touching in the top right corner will deactivate the program and it will need to be relaunched from KUAL.


It's actually grabbing all touchscreen events which is why it won't translate complex gestures, I wanted to keep things simple and I am not well versed into X programming.

Feel free to have fun with it.


It was inspired by the [untouchable](http://www.fabiszewski.net/kindle-untouchable/) extension by [bfabiszewski](https://github.com/bfabiszewski/)  so let's give the guy credit, I only butchered his code to suit my needs.
