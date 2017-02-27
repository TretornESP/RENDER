//WINAPI BASED OPEN 2D GRAPHIC ENGINE
//By Tretorn
//GNU Licensing @ 2017
//Ask for support at tretornesp@gmail.com
//Version PRE-ALPHA PUBLIC RELEASE (27/02/2017)


// link ldi32, psapi.lib
// add linker options: -lwinmm, -DSAPI_VERSION=1
// Compile with C++11 ISO

//INCLUDES
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif // _WIN32_WINNT

#ifndef _WINDOWS_H
#include <windows.h>
#endif // _WINDOWS_H

#ifndef _GLIBCXX_IOSTREAM
#include <iostream>
#endif // _GLIBCXX_IOSTREAM

#ifndef _MATH_H_
#include <math.h>
#endif // _MATH_H_

#ifndef _STDIO_H_
#include <stdio.h>
#endif // _STDIO_H_

#ifndef _TYPEINFO
#include <typeinfo>
#endif // _TYPEINFO

#ifndef	_TIME_H_
#include <time.h>
#endif // _TIME_H_

#ifndef _PSAPI_H
#include <psapi.h>
#endif // _PSAPI_H

#ifndef _RENDER_H
#define _RENDER_H

#pragma GCC diagnostic ignored "-Wdiv-by-zero"

using namespace std;

//MODIFY AS YOU WISH
const int   MAX_FPS          = 0;    /*MAX FPS DISPLAYED SET TO 0 TO NO LIMIT
setting over 50 may cause performance drops */
const int   KEYBOARD_DELAY   = 2;     //DELAY IN MS OF THE KEYBOARD INPUT INCREASING IS FPS KILLER
const int   WIDTH_CORRECTOR  = 0;     //MODIFY IF SCENE IS NOT DISPLAYED CORRECTLY
const int   HEIGHT_CORRECTOR = 0;
const bool  DISPLAYFPS       = true;  //DISPLAY FPS COUNTER ON A CORNER OF THE SCREEN
const bool  DISPLAYRAM       = true;  //DISPLAY RAM USAGE ON A CORNER OF THE SCREEN
const bool  CANMAXIMIZE      = false; //SET IF PROGRAM WINDOW CAN BE MAXIMIZED
const bool  MOUSECAPTURE     = false; //IF TRUE MOUSE WILL STAY AT THE CENTER OF THE WINDOW
const bool  DEBUG            = true;  //IF FALSE DEBUG WINDOW WILL NOT SHOW UP

const int   PIXEL_SIZE       = 4*8;  //SIZE OF EACH PIXEL IN RAM (DO NOT MODIFY!!!)
const int   MAX_RES_X        = 840; /* MAX WINDOW WIDTH RESOLUTION. LESS USES LESS MEMORY
APROXIMATED VALUES MUST BE THE RESOLUTION + 200
THIS EXAMPLE VALUES ARE SET FOR A 640x480 RESOLUTION GAME. AND MAY CRASH IF render_init_fullscreen() is invoked */
const int   MAX_RES_Y        = 680;
//ENGINE START

int MAP_WIDTH; //SCENE PROPERTIES
int MAP_HEIGHT;

int FPS_i; //RENDER FRAMES PER SECOND
int FPS_c = 1;
char FPS[3];

unsigned int slept=0;

long int _MOUSE_X, _MOUSE_Y; //MOUSE ACTUAL COORDENATES
short int _MOUSE_X_DIR, _MOUSE_Y_DIR; //MOUSE ACTUAL DIRECTION (-1 LEFT OR DOWN, 0 CENTER, +1 RIGHT OR UP)

bool NEED_REDRAW; //TRUE IF WINDOW RESIZED

bool DESTROY; //TRUE IF DESTROYING AN OBJECT

bool _SPACE_PRESSED, _ESCAPE_PRESSED, _RIGHT_PRESSED, _LEFT_PRESSED, _UP_PRESSED, _DOWN_PRESSED,      //KEYBOARD AND MOUSE INPUT CODES
     _RIGHT_MOUSE_PRESSED, _LEFT_MOUSE_PRESSED, _CENTER_MOUSE_PRESSED, _ENTER_PRESSED, _SHIFT_PRESSED,
     _A_PRESSED, _B_PRESSED, _C_PRESSED, _D_PRESSED, _E_PRESSED, _F_PRESSED, _G_PRESSED, _H_PRESSED,
     _I_PRESSED, _J_PRESSED, _K_PRESSED, _L_PRESSED, _M_PRESSED, _N_PRESSED, _O_PRESSED,
     _P_PRESSED, _Q_PRESSED, _R_PRESSED, _S_PRESSED, _T_PRESSED, _U_PRESSED, _V_PRESSED, _W_PRESSED,
     _X_PRESSED, _Y_PRESSED, _Z_PRESSED, _0_PRESSED, _1_PRESSED, _2_PRESSED, _3_PRESSED, _4_PRESSED,
     _5_PRESSED, _6_PRESSED, _7_PRESSED, _8_PRESSED, _9_PRESSED;

POINT _mouse; // POINT STRUCTURE TO MOUSE INFO

HBITMAP map;
HWND hwnd; //HANDLE TO MAIN WINDOW
WNDCLASSEX w; //EXTENDED CLASS OF WINDOW
HDC hdc; //HANDLE TO HDC
MSG msg; //WINDOW MESSAGE

PROCESS_MEMORY_COUNTERS_EX pmc; // PROCESS MEMORY COUNTER FOR MEASURING RAM USAGE

SIZE_T RAM_i;
char RAM[10]; // RAM COUNTER

clock_t t1, t2, t3; //CLOCK FOR FPS CALC

COLORREF *SCENE = (COLORREF*) calloc(MAX_RES_X*MAX_RES_Y, sizeof(COLORREF));    //MAIN SCENE BUFFER
COLORREF *SCENE_TMP = (COLORREF*) calloc(MAX_RES_X*MAX_RES_Y, sizeof(COLORREF)); //SECONDARY SCENE BUFFER

struct RGB_t { //STRUCTURE FOR RGB COLORS
  int R; //RED
  int G; //GREEN
  int B; //BLUE
};

struct _Rectangle { //RECTANGLE STRUCTURE
  bool _solid; // IF FALSE WILL DRAW HOLLOW
  RGB_t _RGB; //COLOR BUFFER
  COORD _LeftUpper; //COORDS OF RECTANGLE CORNERS
  COORD _RightUpper;
  COORD _LeftBottom;
  COORD _RightBottom;
};

struct _Triangle { //TRIANGLE STRUCTURE
  bool _solid;
  RGB_t _RGB;
  COORD _Top; //TOP OF THE TRIANGLE
  COORD _LeftBottom;
  COORD _RightBottom;
};

struct _Line { //LINE STRUCTURE
  RGB_t _RGB;
  COORD _StartPos; //START POINT OF LINE
  COORD _EndPos; //FINISH POINT OF LINE
};

struct _Circle { //CIRCLE STRUCTURE
  bool _solid;
  RGB_t _RGB;
  COORD _Center; //CENTER OF THE CIRCLE
  int _radius; //RADIUS OF THE CIRCLE
};

//PROTOTYPES OF FUNCTIONS (DESCRIPTION ON EACH FUNCTION)
//INVOCABLE FUNCTIONS
void render_init_fullscreen(char const* title);
void render_init(int WIDTH, int HEIGHT, char const* titulo);
void render_render();
void render_quit();
void render_pixel(int WIDTH, int HEIGHT, int RED, int GREEN, int BLUE);
void render_pixel_tmp(int WIDTH, int HEIGHT, int RED, int GREEN, int BLUE);
void render_fill(int RED, int GREEN, int BLUE);
void render_reset();
void render_reset_pixel(int WIDTH, int HEIGHT);
void render_load();
void render_playsound(const char* file);
void render_draw_shape(_Line shape);
void render_draw_shape(_Triangle shape);
void render_draw_shape(_Rectangle shape);
void render_draw_shape(_Circle shape);
_Line render_createobject_line(int color_r, int color_g, int color_b,
                               int startpos_x, int startpos_y, int finishpos_x, int finishpos_y);
_Triangle render_createobject_triangle(bool solid, int color_r, int color_g, int color_b,
                                       int top_x, int top_y, int left_x, int left_y, int right_x, int right_y);
_Rectangle render_createobject_rectangle(bool solid, int color_r, int color_g, int color_b,
                                   int LeftUpper_x, int LeftUpper_y, int RightUpper_x, int RightUpper_y,
                                   int LeftBottom_x, int LeftBottom_y, int RightBottom_x, int RightBottom_y);
_Circle render_createobject_circle(bool solid, int color_r, int color_g, int color_b,
                                int center_x, int center_y, int radius);
void render_loop();
void input();
//MAINLY INTERNAL FUNCTIONS
void triangle_to_line(_Line shape);
long __stdcall WindowProcedure( HWND window, unsigned int msg, WPARAM wp, LPARAM lp );

//UNDRAWS OBJECT BY GENERATING A BLACK OBJECT
template <typename T>
void render_destroy_shape(T shape) {
  DESTROY=true;
  render_draw_shape(shape);
  DESTROY=false;
  cout << "[DEBUG] " << typeid(T).name() << " FILLED WITH BLACK" << endl;
}

template <typename T>
void render_recolor_shape(T shape, int R, int G, int B) {
  shape._RGB.R=R;
  shape._RGB.G=G;
  shape._RGB.B=B;
  render_draw_shape(shape);
  cout << "[DEBUG] " << typeid(T).name() << " RECOLORED TO " << " (" << R << ", " << G << ", " << B << ")" << endl;
}

void render_init_fullscreen(char const* title) {
  cout << "[DEBUG] +------------------------------------" << endl;
  cout << "[DEBUG] |    INITIAL RENDER CONFIG           " << endl;
  cout << "[DEBUG] | MAX FPS:                  " << MAX_FPS << endl;
  cout << "[DEBUG] | KEYBOARD DELAY:           " << KEYBOARD_DELAY << endl;
  cout << "[DEBUG] | WIDTH / HEIGHY CORRECTOR: " << WIDTH_CORRECTOR << " / " << HEIGHT_CORRECTOR << endl;
  cout << "[DEBUG] | DISPLAY FPS COUNTER:      " << DISPLAYFPS << endl;
  cout << "[DEBUG] | DISPLAY RAM USAGE:        " << DISPLAYRAM << endl;
  cout << "[DEBUG] | CAN MAXIMIZE WINDOW:      " << CANMAXIMIZE << endl;
  cout << "[DEBUG] | MOUSE IS CAPTURED:        " << MOUSECAPTURE << endl;
  cout << "[DEBUG] | DEBUG CONSOLE IS SHOWN:   " << DEBUG << endl;
  cout << "[DEBUG] +------------------------------------" << endl;
  cout << "[DEBUG] | MAX RESOLUTION AVALIABLE IS: " << MAX_RES_X << "x" << MAX_RES_Y << endl;

  if (DEBUG==false) {FreeConsole();}

  HMONITOR hmon = MonitorFromWindow(GetActiveWindow(), MONITOR_DEFAULTTONEAREST);
  MONITORINFO mi = { sizeof(mi) };
  if (!GetMonitorInfo(hmon, &mi)) return;
  hwnd = CreateWindow(TEXT("static"),
        TEXT(title),
        (WS_POPUP | WS_VISIBLE),
        mi.rcMonitor.left,
        mi.rcMonitor.top,
        mi.rcMonitor.right - mi.rcMonitor.left,
        mi.rcMonitor.bottom - mi.rcMonitor.top,
        0, 0, GetModuleHandle(0), 0);
  MAP_WIDTH=mi.rcMonitor.right+WIDTH_CORRECTOR;
  MAP_HEIGHT=mi.rcMonitor.bottom+HEIGHT_CORRECTOR;

  ShowWindow( hwnd, SW_SHOWDEFAULT ) ;
  cout << "[DEBUG] WINDOW CREATED. WIDTH: " << MAP_WIDTH << " HEIGHT: " << MAP_HEIGHT << endl;
  cout << "[DEBUG] MAP SIZE IS: " << (MAP_WIDTH*MAP_HEIGHT*PIXEL_SIZE)/1024 << "MB" << endl;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
  }
  HDC hdc_tmp = GetDC(hwnd);
  hdc = hdc_tmp;

  if (MOUSECAPTURE==true) {ShowCursor(false), SetCapture(hwnd);}

  cout << "[DEBUG] RENDER STARTED FULSCREEN MODE" << endl;
}

void render_init(int WIDTH, int HEIGHT, char const* titulo) {
  cout << "[DEBUG] +------------------------------------" << endl;
  cout << "[DEBUG] |    INITIAL RENDER CONFIG           " << endl;
  cout << "[DEBUG] | MAX FPS:                  " << MAX_FPS << endl;
  cout << "[DEBUG] | KEYBOARD DELAY:           " << KEYBOARD_DELAY << endl;
  cout << "[DEBUG] | WIDTH / HEIGHY CORRECTOR: " << WIDTH_CORRECTOR << " / " << HEIGHT_CORRECTOR << endl;
  cout << "[DEBUG] | DISPLAY FPS COUNTER:      " << DISPLAYFPS << endl;
  cout << "[DEBUG] | DISPLAY RAM USAGE:        " << DISPLAYRAM << endl;
  cout << "[DEBUG] | CAN MAXIMIZE WINDOW:      " << CANMAXIMIZE << endl;
  cout << "[DEBUG] | MOUSE IS CAPTURED:        " << MOUSECAPTURE << endl;
  cout << "[DEBUG] | DEBUG CONSOLE IS SHOWN:   " << DEBUG << endl;
  cout << "[DEBUG] +------------------------------------" << endl;
  cout << "[DEBUG] | MAX RESOLUTION AVALIABLE IS: " << MAX_RES_X << "x" << MAX_RES_Y << endl;

  if (DEBUG==false) {FreeConsole();}

  HEIGHT=HEIGHT+HEIGHT_CORRECTOR;
  WIDTH=WIDTH+WIDTH_CORRECTOR;

  MAP_WIDTH = WIDTH;
  MAP_HEIGHT = HEIGHT;

  const char* const myclass = "myclass" ;
  w = { sizeof(WNDCLASSEX), (CS_HREDRAW|CS_VREDRAW), WindowProcedure,
                            0, 0, GetModuleHandle(0), NULL,
                            NULL, NULL,
                            0, myclass, NULL } ;



  RegisterClassEx(&w);
  if (CANMAXIMIZE==true) {
    hwnd = CreateWindowEx( 0, myclass, titulo,
                            (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX),
                            CW_USEDEFAULT, CW_USEDEFAULT,
                            MAP_WIDTH, MAP_HEIGHT, 0, 0, GetModuleHandle(0), 0 );

  } else {
   hwnd = CreateWindowEx( 0, myclass, titulo,
                            (WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX),
                            CW_USEDEFAULT, CW_USEDEFAULT,
                            MAP_WIDTH, MAP_HEIGHT, 0, 0, GetModuleHandle(0), 0 );
  }
  ShowWindow( hwnd, SW_SHOWDEFAULT ) ;
  cout << "[DEBUG] WINDOW CREATED. WIDTH: " << MAP_WIDTH << " HEIGHT: " << MAP_HEIGHT << endl;
  cout << "[DEBUG] MAP SIZE IS: " << (MAP_WIDTH*MAP_HEIGHT*PIXEL_SIZE)/1024 << "MB" << endl;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
  }
  HDC hdc_tmp = GetDC(hwnd);
  hdc = hdc_tmp;

  if (MOUSECAPTURE==true) {ShowCursor(false); SetCapture(hwnd);}
  cout << "[DEBUG] RENDER STARTED WINDOWED MODE" << endl;
}

void render_render() {
  t2 = clock();
  if (t1==0) {t1=clock();}
  GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
  RAM_i = pmc.PrivateUsage;
  if (FPS_i <= 999) {sprintf(FPS, "%d", FPS_i);}
  if (RAM_i <= 9999999999) {sprintf(RAM, "%lu", RAM_i);}
  map = CreateBitmap(MAP_WIDTH,
                             MAP_HEIGHT,
                             1,
                             PIXEL_SIZE,
                             (void*) SCENE);

  HDC src = CreateCompatibleDC(hdc);
  SelectObject(src, map);
  if (DISPLAYFPS==true) {
    TextOut(src,          // manipulador de contexto de dispositivo
            0,      // coordenada x de la posición de inicio
            0,      // coordenada y de la posición de inicio
            FPS, // dirección de la cadena
            3);// número de caracteres en la cadena
  }
  if (DISPLAYRAM==true) {
    TextOut(src,
            0,
            15,
            RAM,
            10);
  }
  BitBlt(hdc,
         0,
         0,
         MAP_WIDTH,
         MAP_HEIGHT,
         src,
         0,
         0,
         SRCCOPY);

  DeleteDC(src);
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
  }
  FPS_c++;
  input();
  Sleep(KEYBOARD_DELAY);
  if (((double)(t2 - t1) / CLOCKS_PER_SEC)>=1) {t1=clock(); FPS_i=FPS_c; FPS_c=0;}
  if (MAX_FPS!=0) {
    t3 = clock();
    if ((double)((t3-t2) / CLOCKS_PER_SEC * 1000.0)<(1000/MAX_FPS)) {
      Sleep((int)((double)(1000/MAX_FPS)-(double)((t3-t2) / CLOCKS_PER_SEC * 1000.0)));
    }
  }
}

void render_quit() {
  cout << "[DEBUG] STOPPING RENDER MODULE" << endl;
  PostQuitMessage(0);
  ReleaseDC(hwnd, hdc);
  DeleteDC(hdc);
  exit(0);
}

void render_pixel(int WIDTH, int HEIGHT, int RED, int GREEN, int BLUE) {
  cout << "[DEBUG] PIXEL DRAWN: X: " << WIDTH << " Y: " << HEIGHT << " RGB (" << RED << " ," << GREEN << ", " << BLUE << " )" << endl;
  if (HEIGHT <= 0 || HEIGHT >= MAP_HEIGHT || WIDTH <= 0 || WIDTH >= MAP_WIDTH) {return;}
  SCENE[(MAP_WIDTH*HEIGHT)+WIDTH] = RGB(BLUE, GREEN, RED);
}

void render_pixel_tmp(int WIDTH, int HEIGHT, int RED, int GREEN, int BLUE) {
  cout << "[DEBUG] DRAWN ON TMP BUFFER: X: " << WIDTH << " Y: " << HEIGHT << " RGB (" << RED << " ," << GREEN << ", " << BLUE << " )" << endl;
  if (HEIGHT <= 0 || HEIGHT >= MAP_HEIGHT || WIDTH <= 0 || WIDTH >= MAP_WIDTH) {return;}
  SCENE_TMP[(HEIGHT*MAP_WIDTH)+WIDTH] = RGB(BLUE, GREEN, RED);
}

void render_fill(int RED, int GREEN, int BLUE) {
  cout << "[DEBUG] SCREEN FILLED WITH RGB: (" << RED << " ," << GREEN << ", " << BLUE << " )" << endl;
  for (int i = 0; i < MAP_HEIGHT; i++) {
    for (int j = 0; j < MAP_WIDTH; j++) {
      SCENE[(MAP_WIDTH*i)+j] = RGB(BLUE, GREEN, RED);
    }
  }
}

void render_reset() {
  cout << "[DEBUG] SCREEN SET BACK TO BLACK" << endl;
  for (int i = 0; i < MAP_HEIGHT; i++) {
    for (int j = 0; j < MAP_WIDTH; j++) {
      SCENE[(MAP_WIDTH*i)+j] = RGB(0, 0, 0);
    }
  }
}

void render_reset_pixel(int WIDTH, int HEIGHT) {
  cout << "[DEBUG] PIXEL X: " << WIDTH << " Y: " << HEIGHT << " SET TO BLACK" << endl;
  if (HEIGHT <= 0 || HEIGHT >= MAP_HEIGHT || WIDTH <= 0 || WIDTH >= MAP_WIDTH) {return;}
  SCENE[(HEIGHT*MAP_WIDTH)+WIDTH] = RGB(0, 0, 0);
}

void render_load() {
  cout << "[DEBUG] TMP BUFFER LOADED" << endl;
  for (int i = 0; i < MAP_HEIGHT; i++) {
    for (int j = 0; j < MAP_WIDTH; j++) {
      SCENE[(MAP_WIDTH*i)+j] = SCENE_TMP[(MAP_WIDTH*i)+j];
    }
  }
}

void render_playsound(const char* file) {
  cout << "[DEBUG] SOUND PLAYED " << file << endl;
  PlaySound(NULL, 0, 0);
  PlaySound((LPCSTR) file, NULL, (SND_FILENAME | SND_ASYNC));
}

void render_draw_shape(_Line shape) {
  if (DESTROY==true) {cout << "[DEBUG] DESTROYING LINE OBJECT" << endl;}
  int X;
  int Y;
  int tmp;
  if (shape._StartPos.X > shape._EndPos.X) {
    tmp = shape._StartPos.X;
    shape._StartPos.X = shape._EndPos.X;
    shape._EndPos.X = tmp;
    tmp = shape._StartPos.Y;
    shape._StartPos.Y = shape._EndPos.Y;
    shape._EndPos.Y = tmp;
  }
  int X1=shape._StartPos.X;
  int X2=shape._EndPos.X;
  int Y1=shape._StartPos.Y;
  int Y2=shape._EndPos.Y;
  if (shape._StartPos.X==shape._EndPos.X) {
    if (shape._StartPos.Y==shape._EndPos.Y) {
      if (DESTROY==true) {
        SCENE[MAP_WIDTH*shape._StartPos.Y+shape._StartPos.X] = SCENE_TMP[MAP_WIDTH*shape._StartPos.Y+shape._StartPos.X];
      } else {
        SCENE_TMP[MAP_WIDTH*shape._StartPos.Y+shape._StartPos.X] = SCENE[MAP_WIDTH*shape._StartPos.Y+shape._StartPos.X];
        SCENE[MAP_WIDTH*shape._StartPos.Y+shape._StartPos.X] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
      }
    } else {
      for (int i = shape._StartPos.Y; i <= shape._EndPos.Y; i++) {
        if (DESTROY==true) {
          SCENE[MAP_WIDTH*i+shape._StartPos.X] = SCENE_TMP[MAP_WIDTH*i+shape._StartPos.X];
        } else {
          SCENE_TMP[MAP_WIDTH*i+shape._StartPos.X] = SCENE[MAP_WIDTH*i+shape._StartPos.X];
          SCENE[MAP_WIDTH*i+shape._StartPos.X] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
        }
      }
    }
  } else if (shape._StartPos.Y==shape._EndPos.Y) {
      for (int i = shape._StartPos.X; i <= shape._EndPos.X; i++) {
        if (DESTROY==true) {
          SCENE[MAP_WIDTH*shape._StartPos.Y+i] = SCENE_TMP[MAP_WIDTH*shape._StartPos.Y+i];
        } else {
          SCENE_TMP[MAP_WIDTH*shape._StartPos.Y+i] = SCENE[MAP_WIDTH*shape._StartPos.Y+i];
          SCENE[MAP_WIDTH*shape._StartPos.Y+i] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
        }
      }
  } else {
    for (int i = shape._StartPos.X; i <= shape._EndPos.X; i++) {
      X = i;
      Y = (((X-X1)*(Y2-Y1))/(X2-X1))+Y1;
      cout << "[DEBUG] STARTING LINE CALCULATIONS" << endl;
      cout << "[DEBUG] X: " << X << " X1: " << X1 << " X2: " << X2 << " Y1: " << Y1 << " Y2 " << Y2 << endl;
      cout << "[DEBUG] (((X-X1)*(Y2-Y1))/(X2-X1))+Y1: " << (((X-X1)*(Y2-Y1))/(X2-X1))+Y1 << endl;
      cout << "[DEBUG] ----------------------------------------------" << endl;
      cout << "[DEBUG] X:" << X << "Y:" << Y << endl;
      if (DESTROY==true) {
        SCENE[MAP_WIDTH*Y+X] = SCENE_TMP[MAP_WIDTH*Y+X];
      } else {
        SCENE_TMP[MAP_WIDTH*Y+X] = SCENE[MAP_WIDTH*Y+X];
        SCENE[MAP_WIDTH*Y+X] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
      }
    }
  }
}

void render_draw_shape(_Triangle shape) {
  _Line tmp;
  cout << "[DEBUG] STARTING TRIANGLE CALCULATIONS" << endl;
  if (shape._solid==false) {
    cout << "[DEBUG] *****************************" << endl;
    cout << "[DEBUG] -----------------------------" << endl;
    cout << "[DEBUG] " << "L1 Left(" << shape._LeftBottom.X << " ," << shape._LeftBottom.Y <<") Right (" << shape._RightBottom.X << " ," << shape._LeftBottom.Y << ")" << endl;
    cout << "[DEBUG] -----------------------------" << endl;
    cout << "[DEBUG] " << "L2 Left(" << shape._LeftBottom.X << " ," << shape._LeftBottom.Y <<") Top (" << shape._Top.X << " ," << shape._Top.Y << ")" << endl;
    cout << "[DEBUG] -----------------------------" << endl;
    cout << "[DEBUG] " << "L3 Right(" << shape._RightBottom.X << " ," << shape._RightBottom.Y <<") Top (" << shape._Top.X << " ," << shape._Top.Y << ")" << endl;
    tmp = render_createobject_line(shape._RGB.R, shape._RGB.G, shape._RGB.B, shape._LeftBottom.X, shape._LeftBottom.Y, shape._RightBottom.X, shape._RightBottom.Y);
    triangle_to_line(tmp);
    tmp = render_createobject_line(shape._RGB.R, shape._RGB.G, shape._RGB.B, shape._LeftBottom.X, shape._LeftBottom.Y, shape._Top.X, shape._Top.Y);
    triangle_to_line(tmp);
    tmp = render_createobject_line(shape._RGB.R, shape._RGB.G, shape._RGB.B, shape._RightBottom.X, shape._RightBottom.Y, shape._Top.X, shape._Top.Y);
    triangle_to_line(tmp);
    cout << "[DEBUG] " << "TOP: (" << shape._Top.X << ", " << shape._Top.Y << ")" << endl;
    cout << "[DEBUG] " << "LEFT: (" << shape._LeftBottom.X << ", " << shape._LeftBottom.Y << ")" << endl;
    cout << "[DEBUG] " << "RIGHT: (" << shape._RightBottom.X << ", " << shape._RightBottom.Y << ")" << endl;
  } else {
    cout << "[DEBUG] *****************************" << endl;
    cout << "[DEBUG] -----------------------------" << endl;
    cout << "[DEBUG] " << "L1 Left(" << shape._LeftBottom.X << " ," << shape._LeftBottom.Y <<") Right (" << shape._RightBottom.X << " ," << shape._LeftBottom.Y << ")" << endl;
    cout << "[DEBUG] -----------------------------" << endl;
    cout << "[DEBUG] " << "L2 Left(" << shape._LeftBottom.X << " ," << shape._LeftBottom.Y <<") Top (" << shape._Top.X << " ," << shape._Top.Y << ")" << endl;
    cout << "[DEBUG] -----------------------------" << endl;
    cout << "[DEBUG] " << "L3 Right(" << shape._RightBottom.X << " ," << shape._RightBottom.Y <<") Top (" << shape._Top.X << " ," << shape._Top.Y << ")" << endl;
    int X;
    int Y;
    int X1=shape._LeftBottom.X;
    int X2=shape._Top.X;
    int Y1=shape._LeftBottom.Y;
    int Y2=shape._Top.Y;
    for (int i = shape._LeftBottom.X; i <= shape._Top.X; i++) {
      X = i;
      Y = (((X-X1)*(Y2-Y1))/(X2-X1))+Y1;
      for (int j = Y; j <= shape._LeftBottom.Y; j++) {
        if (DESTROY==true) {
          SCENE[MAP_WIDTH*j+X] = SCENE_TMP[MAP_WIDTH*j+X];
        } else {
          SCENE_TMP[MAP_WIDTH*j+X] = SCENE[MAP_WIDTH*j+X];
          SCENE[MAP_WIDTH*j+X] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
        }
      }
    }
    X1=shape._Top.X;
    X2=shape._RightBottom.X;
    Y1=shape._Top.Y;
    Y2=shape._RightBottom.Y;
    for (int i = shape._Top.X; i <= shape._RightBottom.X; i++) {
      X = i;
      Y = (((X-X1)*(Y2-Y1))/(X2-X1))+Y1;
      for (int j = Y; j <= shape._LeftBottom.Y; j++) {
        if (DESTROY==true) {
          SCENE[MAP_WIDTH*j+X] = SCENE_TMP[MAP_WIDTH*j+X];
        } else {
          SCENE_TMP[MAP_WIDTH*j+X] = SCENE[MAP_WIDTH*j+X];
          SCENE[MAP_WIDTH*j+X] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
        }
      }
    }
    cout << "[DEBUG] " << "TOP: (" << shape._Top.X << ", " << shape._Top.Y << ")" << endl;
    cout << "[DEBUG] " << "LEFT: (" << shape._LeftBottom.X << ", " << shape._LeftBottom.Y << ")" << endl;
    cout << "[DEBUG] " << "RIGHT: (" << shape._RightBottom.X << ", " << shape._RightBottom.Y << ")" << endl;
  }
}

void render_draw_shape(_Rectangle shape) {
  cout << "[DEBUG] STARTING RECTANGLE CALCULATIONS" << endl;
  if (shape._solid==true) {
    for (int i = shape._LeftUpper.Y; i <= shape._RightBottom.Y; i++) {
      for (int j = shape._LeftUpper.X; j <= shape._RightBottom.X; j++) {
        if (DESTROY==true) {
          SCENE[MAP_WIDTH*i+j] =  SCENE_TMP[MAP_WIDTH*i+j];
        } else {
          SCENE_TMP[MAP_WIDTH*i+j] =  SCENE[MAP_WIDTH*i+j];
          SCENE[MAP_WIDTH*i+j] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
        }
      }
    }
  } else {
    for (int i = shape._LeftUpper.Y; i <= shape._RightBottom.Y; i++) {
      if (DESTROY==true) {
        SCENE[MAP_WIDTH*i+shape._LeftUpper.X] = SCENE_TMP[MAP_WIDTH*i+shape._LeftUpper.X];
        SCENE[MAP_WIDTH*i+shape._RightUpper.X] = SCENE_TMP[MAP_WIDTH*i+shape._RightUpper.X];
      } else {
        SCENE_TMP[MAP_WIDTH*i+shape._LeftUpper.X] = SCENE[MAP_WIDTH*i+shape._LeftUpper.X];
        SCENE[MAP_WIDTH*i+shape._LeftUpper.X] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
        SCENE_TMP[MAP_WIDTH*i+shape._RightUpper.X] = SCENE[MAP_WIDTH*i+shape._RightUpper.X];
        SCENE[MAP_WIDTH*i+shape._RightUpper.X] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
      }
    }
    for (int i = shape._LeftUpper.X; i <= shape._RightBottom.X; i++) {
      if (DESTROY==true) {
        SCENE[MAP_WIDTH*shape._LeftUpper.Y+i] = SCENE_TMP[MAP_WIDTH*shape._LeftUpper.Y+i];
        SCENE[MAP_WIDTH*shape._LeftBottom.Y+i] = SCENE_TMP[MAP_WIDTH*shape._LeftBottom.Y+i];
      } else {
        SCENE_TMP[MAP_WIDTH*shape._LeftUpper.Y+i] = SCENE[MAP_WIDTH*shape._LeftUpper.Y+i];
        SCENE[MAP_WIDTH*shape._LeftUpper.Y+i] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
        SCENE_TMP[MAP_WIDTH*shape._LeftBottom.Y+i] = SCENE[MAP_WIDTH*shape._LeftBottom.Y+i];
        SCENE[MAP_WIDTH*shape._LeftBottom.Y+i] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
      }
    }
  }
}

void render_draw_shape(_Circle shape) {
  cout << "[DEBUG] STARTING CIRCLE CALCULATIONS" << endl;
  int R=shape._radius;
  int CX=shape._Center.X;
  int CY=shape._Center.Y;
  int X, Y;
  int Y_tmp=0;
  for (int i = (CX-R); i <= (CX+R); i++){
    X=i;
    Y=sqrt((R*R)-(X-CX)*(X-CX))+CY;
    if (Y_tmp==0) {Y_tmp=Y;}
    cout << "[DEBUG] X: " << X << " R: " << R << " CX: " << CX << " CY: " << CY << endl;
    cout << "[DEBUG] R^2: " << R*R << endl;
    cout << "[DEBUG] CX-X: " << CX-X << endl;
    cout << "[DEBUG] (X-CX)^2: " << (X-CX)*(X-CX) << endl;
    cout << "[DEBUG] (R^2)-((X-CX)^2): " << R*R-((X-CX)*(X-CX)) << endl;
    cout << "[DEBUG] sqrt((R^2)-((X-CX)^2): " << sqrt((R*R)-(X-CX)*(X-CX)) << endl;
    cout << "[DEBUG] sqrt((R^2)-((X-CX)^2)+CY: " << sqrt((R*R)-(X-CX)*(X-CX))+CY << endl;
    cout << "[DEBUG] X:" << X << "Y:" << Y << endl;
    if (shape._solid==true) {
      if (Y>Y_tmp) {
        for (int j = Y; j >= Y_tmp; j--) {
          Y = j;
          if (DESTROY==true) {
            SCENE[MAP_WIDTH*Y+X] = SCENE_TMP[MAP_WIDTH*Y+X];
            SCENE[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X] = SCENE_TMP[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X];
          } else {
            SCENE_TMP[MAP_WIDTH*Y+X] = SCENE[MAP_WIDTH*Y+X];
            SCENE[MAP_WIDTH*Y+X] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
            SCENE_TMP[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X] = SCENE[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X];
            SCENE[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
          }
        }
      } else if (Y<Y_tmp) {
        for (int j = Y; j <= Y_tmp; j++) {
          Y = j;
          if (DESTROY==true) {
            SCENE[MAP_WIDTH*Y+X] = SCENE_TMP[MAP_WIDTH*Y+X];
            SCENE[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X] = SCENE_TMP[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X];
          } else {
            SCENE_TMP[MAP_WIDTH*Y+X] = SCENE[MAP_WIDTH*Y+X];
            SCENE[MAP_WIDTH*Y+X] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
            SCENE_TMP[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X] = SCENE[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X];
            SCENE[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
          }
        }
      } else {
        if (DESTROY==true) {
          SCENE[MAP_WIDTH*Y+X] = SCENE_TMP[MAP_WIDTH*Y+X];
          SCENE[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X] = SCENE_TMP[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X];
        } else {
          SCENE_TMP[MAP_WIDTH*Y+X] = SCENE[MAP_WIDTH*Y+X];
          SCENE[MAP_WIDTH*Y+X] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
          SCENE_TMP[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X] = SCENE[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X];
          SCENE[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
        }
      }
      Y_tmp = Y;
    } else {
      if (DESTROY==true) {
        SCENE[MAP_WIDTH*Y+X] = SCENE_TMP[MAP_WIDTH*Y+X];
        SCENE[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X] = SCENE_TMP[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X];
      } else {
        SCENE_TMP[MAP_WIDTH*Y+X] = SCENE[MAP_WIDTH*Y+X];
        SCENE[MAP_WIDTH*Y+X] = RGB(shape._RGB.R, shape._RGB.G, shape._RGB.B);
        SCENE_TMP[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X] = SCENE[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X];
        SCENE[MAP_WIDTH*(shape._Center.Y-(Y-shape._Center.Y))+X] = RGB(shape._RGB.R, shape._RGB.G, shape._RGB.B);
      }
    }
  }
}

_Line render_createobject_line(int color_r, int color_g, int color_b,
                              int startpos_x, int startpos_y, int finishpos_x, int finishpos_y)
{
  cout << "[DEBUG] CREATING LINE OBJECT" << endl;
  _Line Line;
  Line._RGB.R=color_r;
  Line._RGB.G=color_g;
  Line._RGB.B=color_b;
  Line._StartPos.X=startpos_x;
  Line._StartPos.Y=startpos_y;
  Line._EndPos.X=finishpos_x;
  Line._EndPos.Y=finishpos_y;
  return Line;
}

_Triangle render_createobject_triangle(bool solid, int color_r, int color_g, int color_b,
                                       int top_x, int top_y, int left_x, int left_y, int right_x, int right_y)
{
  cout << "[DEBUG] CREATING TRIANGLE OBJECT" << endl;
  _Triangle Triangle;
  Triangle._solid=solid;
  Triangle._RGB.R=color_r;
  Triangle._RGB.G=color_g;
  Triangle._RGB.B=color_b;
  Triangle._Top.X=top_x;
  Triangle._Top.Y=top_y;
  Triangle._LeftBottom.X=left_x;
  Triangle._LeftBottom.Y=left_y;
  Triangle._RightBottom.X=right_x;
  Triangle._RightBottom.Y=right_y;
  return Triangle;
}

_Rectangle render_createobject_rectangle(bool solid, int color_r, int color_g, int color_b,
                                   int LeftUpper_x, int LeftUpper_y, int RightUpper_x, int RightUpper_y,
                                   int LeftBottom_x, int LeftBottom_y, int RightBottom_x, int RightBottom_y)
{
  cout << "[DEBUG] CREATING RECTANGLE OBJECT" << endl;
  _Rectangle Rect;
  Rect._solid = solid;
  Rect._RGB.R=color_r;
  Rect._RGB.G=color_g;
  Rect._RGB.B=color_b;
  Rect._LeftUpper.X=LeftUpper_x;
  Rect._LeftUpper.Y=LeftUpper_y;
  Rect._RightUpper.X=RightUpper_x;
  Rect._RightUpper.Y=RightUpper_y;
  Rect._LeftBottom.X=LeftBottom_x;
  Rect._LeftBottom.Y=LeftBottom_y;
  Rect._RightBottom.X=RightBottom_x;
  Rect._RightBottom.Y=RightBottom_y;
  return Rect;
}

_Circle render_createobject_circle(bool solid, int color_r, int color_g, int color_b,
                                int center_x, int center_y, int radius)
{
  cout << "[DEBUG] CREATING CIRCLE OBJECT" << endl;
  _Circle Circle;
  Circle._solid=solid;
  Circle._RGB.R=color_r;
  Circle._RGB.G=color_g;
  Circle._RGB.B=color_b;
  Circle._Center.X=center_x;
  Circle._Center.Y=center_y;
  Circle._radius=radius;
  return Circle;
}

void render_loop() {
  unsigned int slept=0;
  while (slept<(DWORD(1000/MAX_FPS))) {
    input();
    Sleep(KEYBOARD_DELAY);
    slept++;
  }
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
  }
}

#ifndef _INPUT_EXTENDED_H
void input() {

  long int _MOUSE_X_tmp, _MOUSE_Y_tmp;

  GetCursorPos(&_mouse);
  if (MOUSECAPTURE==true) {
    SetCursorPos(MAP_WIDTH/2, MAP_HEIGHT/2);

    _MOUSE_X_tmp = _mouse.x;
    _MOUSE_Y_tmp = _mouse.y;

    if (_MOUSE_X_tmp>MAP_WIDTH/2) {_MOUSE_X_DIR=1;}
    if (_MOUSE_X_tmp<MAP_WIDTH/2) {_MOUSE_X_DIR=-1;}
    if (_MOUSE_X_tmp==MAP_WIDTH/2) {_MOUSE_X_DIR=0;}
    if (_MOUSE_Y_tmp>MAP_HEIGHT/2) {_MOUSE_Y_DIR=1;}
    if (_MOUSE_Y_tmp<MAP_HEIGHT/2) {_MOUSE_Y_DIR=-1;}
    if (_MOUSE_Y_tmp==MAP_HEIGHT/2) {_MOUSE_Y_DIR=0;}
  } else {
    _MOUSE_X_tmp = _mouse.x;
    _MOUSE_Y_tmp = _mouse.y;

    if (_MOUSE_X_tmp>_MOUSE_X) {_MOUSE_X_DIR=1;}
    if (_MOUSE_X_tmp<_MOUSE_X) {_MOUSE_X_DIR=-1;}
    if (_MOUSE_X_tmp==_MOUSE_X) {_MOUSE_X_DIR=0;}
    if (_MOUSE_Y_tmp>_MOUSE_Y) {_MOUSE_Y_DIR=1;}
    if (_MOUSE_Y_tmp<_MOUSE_Y) {_MOUSE_Y_DIR=-1;}
    if (_MOUSE_Y_tmp==_MOUSE_Y) {_MOUSE_Y_DIR=0;}
  }

  _MOUSE_X=_MOUSE_X_tmp;
  _MOUSE_Y=_MOUSE_Y_tmp;

  if (GetAsyncKeyState(VK_LBUTTON)) {_LEFT_MOUSE_PRESSED=true;} else {_LEFT_MOUSE_PRESSED=false;}
  if (GetAsyncKeyState(VK_RBUTTON)) {_RIGHT_MOUSE_PRESSED=true;} else {_RIGHT_MOUSE_PRESSED=false;}
  if (GetAsyncKeyState(VK_MBUTTON)) {_CENTER_MOUSE_PRESSED=true;} else {_CENTER_MOUSE_PRESSED=false;}

  if (GetAsyncKeyState(VK_SPACE)) {_SPACE_PRESSED=true;} else {_SPACE_PRESSED=false;}
  if (GetAsyncKeyState(VK_ESCAPE)) {_ESCAPE_PRESSED=true;} else {_ESCAPE_PRESSED=false;}
  if (GetAsyncKeyState(VK_LEFT)) {_LEFT_PRESSED=true;} else {_LEFT_PRESSED=false;}
  if (GetAsyncKeyState(VK_RIGHT)) {_RIGHT_PRESSED=true;} else {_RIGHT_PRESSED=false;}
  if (GetAsyncKeyState(VK_UP)) {_UP_PRESSED=true;} else {_UP_PRESSED=false;}
  if (GetAsyncKeyState(VK_DOWN)) {_DOWN_PRESSED=true;} else {_DOWN_PRESSED=false;}
  if (GetAsyncKeyState(VK_RETURN)) {_ENTER_PRESSED=true;} else {_ENTER_PRESSED=false;}
  if (GetAsyncKeyState(VK_SHIFT)) {_SHIFT_PRESSED=true;} else {_SHIFT_PRESSED=false;}

  if (GetAsyncKeyState('A')) {_A_PRESSED=true;} else {_A_PRESSED=false;}
  if (GetAsyncKeyState('B')) {_B_PRESSED=true;} else {_B_PRESSED=false;}
  if (GetAsyncKeyState('C')) {_C_PRESSED=true;} else {_C_PRESSED=false;}
  if (GetAsyncKeyState('D')) {_D_PRESSED=true;} else {_D_PRESSED=false;}
  if (GetAsyncKeyState('E')) {_E_PRESSED=true;} else {_E_PRESSED=false;}
  if (GetAsyncKeyState('F')) {_F_PRESSED=true;} else {_F_PRESSED=false;}
  if (GetAsyncKeyState('G')) {_G_PRESSED=true;} else {_G_PRESSED=false;}
  if (GetAsyncKeyState('H')) {_H_PRESSED=true;} else {_H_PRESSED=false;}
  if (GetAsyncKeyState('I')) {_I_PRESSED=true;} else {_I_PRESSED=false;}
  if (GetAsyncKeyState('J')) {_J_PRESSED=true;} else {_J_PRESSED=false;}
  if (GetAsyncKeyState('K')) {_K_PRESSED=true;} else {_K_PRESSED=false;}
  if (GetAsyncKeyState('L')) {_L_PRESSED=true;} else {_L_PRESSED=false;}
  if (GetAsyncKeyState('M')) {_M_PRESSED=true;} else {_M_PRESSED=false;}
  if (GetAsyncKeyState('N')) {_N_PRESSED=true;} else {_N_PRESSED=false;}
  if (GetAsyncKeyState('O')) {_O_PRESSED=true;} else {_O_PRESSED=false;}
  if (GetAsyncKeyState('P')) {_P_PRESSED=true;} else {_P_PRESSED=false;}
  if (GetAsyncKeyState('Q')) {_Q_PRESSED=true;} else {_Q_PRESSED=false;}
  if (GetAsyncKeyState('R')) {_R_PRESSED=true;} else {_R_PRESSED=false;}
  if (GetAsyncKeyState('S')) {_S_PRESSED=true;} else {_S_PRESSED=false;}
  if (GetAsyncKeyState('T')) {_T_PRESSED=true;} else {_T_PRESSED=false;}
  if (GetAsyncKeyState('U')) {_U_PRESSED=true;} else {_U_PRESSED=false;}
  if (GetAsyncKeyState('V')) {_V_PRESSED=true;} else {_V_PRESSED=false;}
  if (GetAsyncKeyState('W')) {_W_PRESSED=true;} else {_W_PRESSED=false;}
  if (GetAsyncKeyState('X')) {_X_PRESSED=true;} else {_X_PRESSED=false;}
  if (GetAsyncKeyState('Y')) {_Y_PRESSED=true;} else {_Y_PRESSED=false;}
  if (GetAsyncKeyState('Z')) {_Z_PRESSED=true;} else {_Z_PRESSED=false;}
  if (GetAsyncKeyState('0')) {_0_PRESSED=true;} else {_0_PRESSED=false;}
  if (GetAsyncKeyState('1')) {_1_PRESSED=true;} else {_1_PRESSED=false;}
  if (GetAsyncKeyState('2')) {_2_PRESSED=true;} else {_2_PRESSED=false;}
  if (GetAsyncKeyState('3')) {_3_PRESSED=true;} else {_3_PRESSED=false;}
  if (GetAsyncKeyState('4')) {_4_PRESSED=true;} else {_4_PRESSED=false;}
  if (GetAsyncKeyState('5')) {_5_PRESSED=true;} else {_5_PRESSED=false;}
  if (GetAsyncKeyState('6')) {_6_PRESSED=true;} else {_6_PRESSED=false;}
  if (GetAsyncKeyState('7')) {_7_PRESSED=true;} else {_7_PRESSED=false;}
  if (GetAsyncKeyState('8')) {_8_PRESSED=true;} else {_8_PRESSED=false;}
  if (GetAsyncKeyState('9')) {_9_PRESSED=true;} else {_9_PRESSED=false;}
}
#endif // _INPUT_EXTENDED_H

void triangle_to_line(_Line shape) {
  cout << "[DEBUG] TRANSFORMING TRIANGLE TO ITS LINES" << endl;
  int tmp;
  if (shape._StartPos.X > shape._EndPos.X) {
    tmp = shape._StartPos.X;
    shape._StartPos.X = shape._EndPos.X;
    shape._EndPos.X = tmp;
    tmp = shape._StartPos.Y;
    shape._StartPos.Y = shape._EndPos.Y;
    shape._EndPos.Y = tmp;
  }
  int X;
  int Y;
  int X1=shape._StartPos.X;
  int X2=shape._EndPos.X;
  int Y1=shape._StartPos.Y;
  int Y2=shape._EndPos.Y;
  if (shape._StartPos.X==shape._EndPos.X) {
    if (shape._StartPos.Y==shape._EndPos.Y) {
      if (DESTROY==true) {
        SCENE[MAP_WIDTH*shape._StartPos.Y+shape._StartPos.X] = SCENE_TMP[MAP_WIDTH*shape._StartPos.Y+shape._StartPos.X];
      } else {
        SCENE_TMP[MAP_WIDTH*shape._StartPos.Y+shape._StartPos.X] = SCENE[MAP_WIDTH*shape._StartPos.Y+shape._StartPos.X];
        SCENE[MAP_WIDTH*shape._StartPos.Y+shape._StartPos.X] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
      }
    } else {
      for (int i = shape._StartPos.Y; i <= shape._EndPos.Y; i++) {
        if (DESTROY==true) {
          SCENE[MAP_WIDTH*i+shape._StartPos.X] = SCENE_TMP[MAP_WIDTH*i+shape._StartPos.X];
        } else {
          SCENE_TMP[MAP_WIDTH*i+shape._StartPos.X] = SCENE[MAP_WIDTH*i+shape._StartPos.X];
          SCENE[MAP_WIDTH*i+shape._StartPos.X] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
        }
      }
    }
  } else if (shape._StartPos.Y==shape._EndPos.Y) {
      for (int i = shape._StartPos.X; i <= shape._EndPos.X; i++) {
        if (DESTROY==true) {
          SCENE[MAP_WIDTH*shape._StartPos.Y+i] = SCENE_TMP[MAP_WIDTH*shape._StartPos.Y+i];
        } else {
          SCENE_TMP[MAP_WIDTH*shape._StartPos.Y+i] = SCENE[MAP_WIDTH*shape._StartPos.Y+i];
          SCENE[MAP_WIDTH*shape._StartPos.Y+i] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
        }
      }
  } else {
    for (int i = shape._StartPos.X; i <= shape._EndPos.X; i++) {
      X = i;
      Y = (((X-X1)*(Y2-Y1))/(X2-X1))+Y1;
      if (DESTROY==true) {
        SCENE[MAP_WIDTH*Y+X] = SCENE_TMP[MAP_WIDTH*Y+X];
      } else {
        SCENE_TMP[MAP_WIDTH*Y+X] = SCENE[MAP_WIDTH*Y+X];
        SCENE[MAP_WIDTH*Y+X] = RGB(shape._RGB.B, shape._RGB.G, shape._RGB.R);
      }
    }
  }
}

long __stdcall WindowProcedure( HWND window, unsigned int msg, WPARAM wp, LPARAM lp ) {
    switch(msg)
    {
        case WM_DESTROY:
            DestroyWindow(hwnd);
            exit(0);
            return 0L ;
            break;
        case WM_SIZE:
            NEED_REDRAW=true;
            return 0L ;
            break;
    }
    return DefWindowProc( window, msg, wp, lp ) ;

}

#endif // _RENDER_H
