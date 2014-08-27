#ifndef _INC_DGLIB
#define _INC_DGLIB

#include <windows.h>
#include <stdio.h>

#define main Main
#define DB6_KASAN -1
typedef HDC char_far;

#ifdef __cplusplus
extern "C" {
#endif

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
extern char_far gvram; 

int Main(void);
void ML(void);

/* ---------- KeyInput Library ---------- */
int kbhit(void);
int getch(void);
int getch2(unsigned long waittime);
int getch3(int vkkey);

/* ---------- Graphics Library ---------- */
void dg_setvideomode(int mode);
void dg_setscreen(int lx,int ly,int lwidth,int lheight,int lbai,char ltb[]);

void dg_pset(char_far hdc,int x,int y,COLORREF color);
COLORREF dg_pget(char_far hdc,int x,int y);
void dg_gcls(char_far hdc,COLORREF color);
void dg_box(char_far hdc,int x,int y,int width,int height,int paintflg,COLORREF color);
void dg_circle(char_far hdc,int cx,int cy,int r,COLORREF color);
void dg_line(char_far hdc,int x1,int y1,int x2,int y2,COLORREF color);

char_far  dg_loadbmp(char *pfilename);
char_far  dg_createbmp(int width,int height);
void dg_drawbmp(char_far hdc,int x,int y,int width,int height,char_far hMemdc);
void dg_drawbmp2(char_far hdc,int x,int y,int width,int height,char_far hMemdc,int sx,int sy);
void dg_drawbmp3(char_far hdc,int x,int y,int width,int height,char_far hMemdc,int sx,int sy,COLORREF toumei);
void dg_drawbmp4(char_far hdc,int x,int y,int width,int height,
				 char_far hMemdc,int sx,int sy,int swidth,int sheight,COLORREF toumei);
void dg_drawbmp5(char_far hdc,int x,int y,int width,int height,
				 char_far hMemdc,int sx,int sy,int swidth,int sheight,short alpha,COLORREF toumei);
void dg_drawbmp6(char_far hdc,int x,int y,int width,int height,
				 char_far hMemdc,int sx,int sy,int swidth,int sheight,int kaku,COLORREF toumei);
void dg_drawbmp6Ex(HDC hdc,int x,int y,int width,int height,
				 HDC hMemdc,int sx,int sy,int swidth,int sheight,int kaku,COLORREF toumei,int kx,int ky);
void dg_deletebmp(char_far hMemdc);

void dg_printf(int x,int y,COLORREF color,char *format, ...);
void dg_printf2(char_far hdc,int x,int y,COLORREF color,COLORREF backcolor,char *format, ...);
void dg_printf3(char_far hdc,int x,int y,int width,int height,COLORREF color,COLORREF backcolor,char *format, ...);
void dg_setfont(int fw,int fh,char fname[]);
void dg_deletefont(void);

char_far dg_createBB(void);
void dg_deleteBB(char_far hMemdc);
void dg_flip(char_far hdc,char_far hMemdc);

/* ---------- Sound Library ---------- */
void ds_play1(char filename[]);
void ds_play2(char filename[]);
void ds_play3(char filename[]);
void ds_stop(char filename[]);

/* ---------- Mouse Library ---------- */
void dm_show(int mode);
void dm_getpos(int *pmx,int *pmy);
void dm_getb(int *pb);
void dm_setpos(int mx,int my);

/* ---------- Joypad Library ---------- */
int dj_up(unsigned int device_id);
int dj_down(unsigned int device_id);
int dj_left(unsigned int device_id);
int dj_right(unsigned int device_id);
int dj_button( unsigned int device_id , unsigned int button_id );
int dj_init(void);

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef _INC_DGLIB */
