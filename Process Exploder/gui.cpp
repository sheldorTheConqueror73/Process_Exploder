#include "gui.h"



void Gui::console_init()
{
	console_window = GetConsoleWindow();
	console_out = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD largest_size = GetLargestConsoleWindowSize(console_out);
	largest_size.X-=100;
	largest_size.Y-=100;
	SetConsoleScreenBufferSize(console_out, largest_size);
	ShowWindow(console_window, SW_MAXIMIZE);
	unhighlight_row();
}

void Gui::highlight_row()
{
	SetConsoleTextAttribute(console_out, INVERTED_COLOR);
}

void Gui::unhighlight_row()
{
	SetConsoleTextAttribute(console_out, NORMAL_COLOR);
}

char* Gui::file_picker()
{

	char* buffer = (char*)malloc(MAX_PATH * sizeof(char));
	_browseinfoA binfo{
		console_window,
		NULL,
		buffer,
		"Please Select DLL File",
		BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_VALIDATE | BIF_BROWSEINCLUDEFILES,
		NULL,
		NULL
	};
	PCIDLIST_ABSOLUTE  path = SHBrowseForFolderA(&binfo);
	if (!path)
	{
		return NULL;
	}
	return SHGetPathFromIDListA(path, buffer) ? buffer: NULL;


}
void Gui::display_error(char* msg)
{
	MessageBoxA(console_window, msg, NULL, MB_ICONERROR | MB_APPLMODAL);
}

void Gui::close()
{
	exit(0);
	//PostMessage(console_window, WM_QUIT,0,0);
}
