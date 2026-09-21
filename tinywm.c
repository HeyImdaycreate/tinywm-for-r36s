// Полноценный автономный код TinyWM для жесткой компиляции в облаке
#define Mod1Mask (1<<3)
#define GrabModeAsync 1
#define True 1
#define None 0L
#define KeyPress 2
#define ButtonPress 4
#define ButtonRelease 5
#define MotionNotify 6
#define ButtonPressMask (1L<<2)
#define ButtonReleaseMask (1L<<3)
#define PointerMotionMask (1L<<6)

typedef unsigned long XID;
typedef unsigned long Window;
typedef XID KeySym;
typedef unsigned char KeyCode;

typedef struct { void *ext_data; int error_string; } Display;

typedef struct {
    int x, y; int width, height; int border_width; int depth; void *visual;
    Window root; int class; int bit_gravity; int win_gravity; int backing_store;
    unsigned long backing_planes; unsigned long backing_pixel; int save_under;
    void *colormap; int map_installed; int map_state; long all_event_masks;
    long your_event_mask; long do_not_propagate_mask; int override_redirect; void *screen;
} XWindowAttributes;

typedef struct { int type; unsigned long serial; int send_event; Display *display; Window window; Window root; Window subwindow; unsigned long time; int x, y; int x_root, y_root; unsigned int state; unsigned int button; int same_screen; } XButtonEvent;
typedef struct { int type; unsigned long serial; int send_event; Display *display; Window window; Window root; Window subwindow; unsigned long time; int x, y; int x_root, y_root; unsigned int state; unsigned int keycode; int same_screen; } XKeyEvent;
typedef struct { int type; unsigned long serial; int send_event; Display *display; Window window; Window root; Window subwindow; unsigned long time; int x, y; int x_root, y_root; unsigned int state; int is_hint; char same_screen; } XMotionEvent;

typedef union _XEvent { int type; XKeyEvent xkey; XButtonEvent xbutton; XMotionEvent xmotion; long pad; } XEvent;

extern Display *XOpenDisplay(char *); extern Window XDefaultRootWindow(Display *);
extern KeyCode XKeysymToKeycode(Display *, KeySym); extern KeySym XStringToKeysym(char *);
extern int XGrabKey(Display *, int, unsigned int, Window, int, int, int);
extern int XGrabButton(Display *, unsigned int, unsigned int, Window, int, unsigned int, int, int, Window, Window);
extern int XNextEvent(Display *, XEvent *); extern int XRaiseWindow(Display *, Window);
extern int XGetWindowAttributes(Display *, Window, XWindowAttributes *);
extern int XMoveResizeWindow(Display *, Window, int, int, unsigned int, unsigned int);

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(void) {
    Display * dpy; Window root; XWindowAttributes attr; XButtonEvent start; XEvent ev;
    if (!(dpy = XOpenDisplay(0))) return 1;
    root = XDefaultRootWindow(dpy);
    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("F1")), Mod1Mask, root, True, GrabModeAsync, GrabModeAsync);
    XGrabButton(dpy, 1, Mod1Mask, root, True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(dpy, 3, Mod1Mask, root, True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    start.subwindow = None;
    for(;;) {
        XNextEvent(dpy, &ev);
        if(ev.type == KeyPress && ev.xkey.subwindow != None) XRaiseWindow(dpy, ev.xkey.subwindow);
        else if(ev.type == ButtonPress && ev.xbutton.subwindow != None) XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
        else if(ev.type == MotionNotify && start.subwindow != None) {
            int xdiff = ev.xbutton.x_root - start.x; int ydiff = ev.xbutton.y_root - start.y;
            XMoveResizeWindow(dpy, start.subwindow, attr.x + (start.button==1 ? xdiff : 0), attr.y + (start.button==1 ? ydiff : 0),
                MAX(1, attr.width + (start.button==3 ? xdiff : 0)), MAX(1, attr.height + (start.button==3 ? ydiff : 0)));
        }
        else if(ev.type == ButtonRelease) start.subwindow = None;
    }
}
