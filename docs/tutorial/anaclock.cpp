#include <math.h>
#include <time.h>
#include "anaclock.h"
using namespace MAS;

// The constructor: just reset the base class and the timerID
AnaClock::AnaClock()
	:Widget(),
	timerID(-1)
{
}


// Destructor: empty so far
AnaClock::~AnaClock() {
}


// MsgStart(): called when the parent dialog starts executing. Notice that in every
// message handler we call the base class' handler to pass the message up the hierarchy.
// This isn't strictly necessary as some base Widget's handlers are trivial and more or
// less don't do anything, but it's good practice to do so. It can save you a lot of
// hair pulling if you get used to doing this all the time.
void AnaClock::MsgStart() {
	Widget::MsgStart();
	
	// Install a timer that ticks once every 1000 ms and remember its ID for later.
	timerID = Timer::Install(1000);
}


// MsgEnd(): opposite of MsgStart(), called when the dialog stops executing.
void AnaClock::MsgEnd() {
	Widget::MsgEnd();
	
	// Stop our timer.
	Timer::Kill(timerID);
}


// MsgTimer(): out timer handler. Notice that this handler is called whenever any timer
// ticks, so we must check whether the timer that triggered this handler to be called
// is in fact our clock timer or not.
void AnaClock::MsgTimer(int ID) {
	Widget::MsgTimer(ID);
	
	// do the check
	if (ID == timerID) {
		// Because the clock is partly transparent, everything that is behind it has
		// to be redrawn before the clock itself is drawn. This function call makes
		// sure the parent dialog will redraw the area that is occupied by the clock
		// including the clock itslef. If the clock wasn't partly transparent, we
		// would simply call the Redraw() method of this widget instead of dealing
		// with the parent dialog.
		GetParent()->Redraw(*this);
	}
}


// Draw(): the clock's drawing routine that actuall draws the clock widget. The canvas
// bitmap is the bitmap that belongs to the clock itself and not the parent dialog or
// the global screen or whatever. This means that the widget doesn't have to care much
// about where on the canvas it has to draw or how big it is or anything like that.
// The actualy drawing code is quite ugly and won't be commented much. It just does
// some trigonometry and draws the clock's ticks and handles and that's it...
void AnaClock::Draw(MAS::Bitmap &canvas) {
	// some temp variables
	int x1, y1;
	
	// the centre point
	int xc = w()/2;
	int yc = h()/2;
	
	// the radius of the clock
	int r = MIN(xc-4, yc-4);
	
	// draw 12 ticks
	float angle = -90.0f;
	float PI = 3.14159f;
	for (int i=0; i<12; i++) {
		float ca = cos(angle*PI/180.0f);
		float sa = sin(angle*PI/180.0f);
		x1 = (int)(r*ca) + xc;
		y1 = (int)(r*sa) + yc;
		
		// if the clock has focus, fill the tick with green
		if (HasFocus()) {
			canvas.Circlefill(x1, y1, 1, Color::green);
		}
		canvas.Circle(x1, y1, 2, Color::black);
		
		angle += 30.0f;
	}
	
	// get current time
	time_t currentTime = time(NULL);
	struct tm *t = localtime(&currentTime);
	
	// draw the seconds handle
	angle = (-90.0f + (float)t->tm_sec*6.0f)*PI/180.0f;
	x1 = (int)(r*cos(angle)*0.9f) + xc;
	y1 = (int)(r*sin(angle)*0.9f) + yc;
	canvas.Line(xc, yc, x1, y1, Color::black);

	// draw the minutes handle
	angle = (-90.0f + (float)t->tm_min*6.0f)*PI/180.0f;
	x1 = (int)(r*cos(angle)*0.7f) + xc;
	y1 = (int)(r*sin(angle)*0.7f) + yc;
	canvas.Line(xc, yc, x1, y1, Color::black);

	// and draw the hours handle
	angle = (-90.0f + (float)(t->tm_hour%12)*30.0f + (float)t->tm_min/2.0f)*PI/180.0f;
	x1 = (int)(r*cos(angle)*0.5f) + xc;
	y1 = (int)(r*sin(angle)*0.5f) + yc;
	canvas.Line(xc, yc, x1, y1, Color::black);
}


// MsgWantfocus(): simply returns true to indicate the clock indeed does take input focus.
bool AnaClock::MsgWantfocus() {
	Widget::MsgWantfocus();
	return true;
}


// MsgWantmouse(): called when the mouse cursor comes over the widget's bounding box to ask
// if the widget actually does want to accept it. The clock is round so it has to check if
// if the mouse cursor is not only in it's bounding box, but also in it's round area.
bool AnaClock::MsgWantmouse() {
	Widget::MsgWantmouse();
	
	// get the position of the mouse cursor relative to the top left corner of the clock widget
	Point mp = GetMousePos();
	
	// get centre point of the clock
	Point c = Point(w()/2, h()/2);
	
	// the radius
	int r = MIN(c.x(), c.y());
	
	// the vector between the mouse cursor and centre point
	Point d = c - mp;
	
	// see if the length of the vector is less than the radius or not
	if (d.x()*d.x() + d.y()*d.y() < r*r) {
		return true;
	}
	else {
		return false;
	}
}


// MsgLPress(): just sets the D_PRESSED flag to indicate the clock is being pressed down. This
// handler is called whenever the left mouse button is pressed down and the clock widget accepts
// mouse input.
void AnaClock::MsgLPress() {
	Widget::MsgLPress();
	SetFlag(D_PRESSED);
}


// MsgLRelease(): opposite of MsgLPress(). Called when the left mouse button is released.
void AnaClock::MsgLRelease() {
	Widget::MsgLRelease();
	ClearFlag(D_PRESSED);
}


// MsgMousemove(): the handler for handling mouse moves. This is called whenever the mouse
// cursor moves and the clock accepts mouse (i.e. when the mouse is over the clock or the
// D_PRESSED flag is set).
void AnaClock::MsgMousemove(const Point& d) {
	Widget::MsgMousemove(d);
	
	// See if the clock is actually pressed.
	if (TestFlag(D_PRESSED)) {
		// move it
		Move(d);
	}
}


// MsgLostfocus(): called when the clock loses focus. Just requests the parent dialog to redraw
// area occupied by the clock so the old state won't show through.
void AnaClock::MsgLostfocus() {
	GetParent()->Redraw(*this);
	Widget::MsgLostfocus();
}


// MsgChar(): buffered keyboard input handler. Just checks which of the cursor keys are pressed
// and moves the clock accordingly. This handler returns true if the key has been handled and
// false otherwise.
bool AnaClock::MsgChar(int c) {
	if (Widget::MsgChar(c)) return true;
	
	Point d;
	switch (c>>8) {
		case KEY_UP:	d.y(-2);	break;
		case KEY_DOWN:	d.y(2);		break;
		case KEY_LEFT:	d.x(-2);	break;
		case KEY_RIGHT:	d.x(2);		break;
		default: 		return false;
	};
	
	if (key_shifts & KB_SHIFT_FLAG) {
		d.x(d.x()*4);
		d.y(d.y()*4);
	}
	
	Move(d);
	return true;
}


// Actually move the clock by the given distance. Also makes sure it doesn't go off the
// screen to the left or top because that would crash MASkinG.
void AnaClock::Move(const Point &d) {
	// make sure what was behind the clock is redrawn
	GetParent()->Redraw(*this);
	
	// calculate new position
	Point pos = topLeft() + d;
	
	// clip to the screen
	if (pos.x() < 0) pos.x(0);
	if (pos.y() < 0) pos.y(0);
		
	// move the clock to the new position
	Place(pos);
}
