#include "graphics.hpp"
#include "controller.hpp"
#include "views/widget.hpp"
#include "log.hpp"

#include <unistd.h> //let mingw handle it if needed
#include <cassert>
#include <iostream>
#include <vector>
#include <chrono>
#include <deque>

#include <X11/keysym.h>

using namespace std;
using namespace chrono;

struct _XDisplay;
typedef struct _XDisplay Display;

struct Graphics_X : Graphics {
  Graphics_X();

  // Methods
  void drawString(int x, int y, const std::string& str, Context gc = DEFAULT);
  void drawChar(int x, int y, char str, Context gc = DEFAULT);

  void handle_events(struct Controller*);

  void LoadText(const std::string msg, const std::string font_file);
  void repaint();
  void destroy();

  // Data
  int s;
  Display *display;

  Window window;

  GC white_gc;
  XColor white_col;
  Colormap colormap;

};

Graphics_X::Graphics_X() {
  /* open connection with the server */
  display = XOpenDisplay(NULL);
  assert(display != nullptr);
 
  s = DefaultScreen(display);

  /* create window */
  width = 80;
  height = 40;

  window = XCreateSimpleWindow(display, RootWindow(display, s), 10, 10,
                                      width * FONT_WIDTH, height * FONT_HEIGHT, 1,
                                      BlackPixel(display, s), WhitePixel(display, s));

  colormap = DefaultColormap(display, s);
  white_gc = XCreateGC(display, window, 0, 0);
  XParseColor(display, colormap, white, &white_col);
  XAllocColor(display, colormap, &white_col);
  XSetForeground(display, white_gc, white_col.pixel);

  /* select kind of events we are interested in */
  XSelectInput(display, window,
               ExposureMask
               | KeyPressMask
               | StructureNotifyMask);
 
  /* map (show) the window */
  XMapWindow(display, window);
}
void Graphics_X::LoadText(const std::string msg, const std::string font_file)
{
  (void)msg;
  (void)font_file;
  return;
}
void Graphics_X::handle_events(Controller* c) {
  XEvent event;

  int events = XPending(display);

  while (events > 0) {
    LOGGER::debug() << events << " Events." << endl;
    XNextEvent(display, &event);
 
    switch (event.type) {
    case Expose:
//      if (event.xexpose.count == 0)
//        repaint();
      break;
    case KeyPress: {
      auto keycode = event.xkey.keycode;
      int keysyms_per_keycode_return;
      auto keysyms = XGetKeyboardMapping(display, keycode, 1, &keysyms_per_keycode_return);
      c->handle_keypress(keysyms[0]);
      XFree(keysyms);
      if (destroyed) return;
      break;
    }
    case ConfigureNotify:
      width = event.xconfigure.width / FONT_WIDTH;
      height = event.xconfigure.height / FONT_HEIGHT;
      break;
    default:
      cerr << "Unhandled event: " << event.type << endl;
      break;
    }

    events = XPending(display);
  }
}
void Graphics_X::drawString(int x, int y, const string & str, const Graphics_X::Context gc) {
  GC* t;
  switch (gc) {
  case WHITE:
    t = &white_gc;
    break;
  default:
    t = &DefaultGC(display, s);
    break;
  }
  XDrawString(display,
              window,
              *t,
              x * FONT_WIDTH,
              y * FONT_HEIGHT,
              str.c_str(),
              str.length());
}

void Graphics_X::drawChar(int x, int y, char ch, const Graphics_X::Context gc) {
  GC* t;
  switch (gc) {
  case WHITE:
    t = &white_gc;
    break;
  default:
    t = &DefaultGC(display, s);
    break;
  }
  XDrawString(display,
              window,
              *t,
              x * FONT_WIDTH,
              y * FONT_HEIGHT,
              &ch,
              1);
}

void Graphics_X::repaint() {
  XFillRectangle(display, window, white_gc, 0, 0, getWidth() * FONT_WIDTH, getHeight() * FONT_HEIGHT);
  
  for (auto p : c)
    p->render(*this, { 0, 0, getWidth(), getHeight() });
}

void Graphics_X::destroy() {
  if (destroyed) return;
  XCloseDisplay(display);
  destroyed = true;
}

Graphics* new_graphics() { return new Graphics_X(); }

//// Now for the plug functions
void Graphics::drawString(int x, int y, const std::string& str, Context gc) {
  return static_cast<Graphics_X*>(this)->drawString(x,y,str,gc);
}
void Graphics::drawChar(int x, int y, char str, Context gc) {
  return static_cast<Graphics_X*>(this)->drawChar(x,y,str,gc);
}
void Graphics::handle_events(Controller* c) {
  return static_cast<Graphics_X*>(this)->handle_events(c);
}
void Graphics::repaint() {
  return static_cast<Graphics_X*>(this)->repaint();
}
void Graphics::destroy() {
  return static_cast<Graphics_X*>(this)->destroy();
}
