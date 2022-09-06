#pragma once
#include <Windows.h>
#include<Wincon.h>
#include <shlobj_core.h>

#define NORMAL_COLOR 7
#define INVERTED_COLOR 240

class Gui
{
	 HWND console_window;
	 HANDLE console_out;
public:
	 void console_init();
	 void unhighlight_row();
	 void highlight_row();
	 char* file_picker();
	 void display_error(char*);
	 
	 
	 
	 void close();
};

