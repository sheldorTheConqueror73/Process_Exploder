#pragma once
#include "Process_operations.h"
#include "gui.h"

#include <ShlObj_core.h>
#include <direct.h>
#include  <io.h>
#include<conio.h>
constexpr int MAX_PAGE_SIZE = 61;



void inject_dll(char *dll_path,DWORD pid,Logger *logger,Gui *gui)
{
	if (_access(dll_path, 0) != -1)
		Process_operations::inject_dll(pid, dll_path, logger);
	else
	{
		logger->vlog_crit("cannot find dll %s\n", dll_path);
		gui->display_error((char*)"dll file not found");
	}
}

void display_procs(vector<Process_data*> procs,Gui &gui,int  start,int selected_line)
{
	std::system("CLS");
	printf("Showing Processes %d to %d out of %d\n", start, start + MAX_PAGE_SIZE < procs.size() ? start + MAX_PAGE_SIZE - 1 : procs.size() - 1, procs.size() - 1);
	for (int i = start; i < start + MAX_PAGE_SIZE && i < procs.size(); i++)
	{
		if (selected_line == i)
		{
			char temp[1024];
			gui.highlight_row();
			printf("%d\tName: %-35s\tPID: %-7d", i, (procs[i])->name, (procs[i])->pid);


		}
		else
		{
			printf("%d\tName: %-35s\tPID: %-7d", i, (procs[i])->name, (procs[i])->pid);

		}

		gui.unhighlight_row();
		printf("\n");
	}
}

int main(int argc, char*args[])
{
	

	//consule init
	Gui gui;
	gui.console_init();

	//logger init
	size_t count = MAX_PATH;
	char ad_path[MAX_PATH], file_path[MAX_PATH],dir_path[MAX_PATH], dll_path[] = "C:\\Users\\Chuck\\source\\repos\\Process Exploder\\Process Exploder\\PE_DLL.dll";
	SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, ad_path);
	snprintf(dir_path, MAX_PATH,"%s%s", ad_path, "\\PE\\");
	snprintf(file_path, MAX_PATH,"%s%s", ad_path, "\\PE\\log.txt");
	_mkdir(dir_path);
	if (_access(file_path,0) !=-1)
	{
		remove(file_path);
	}
	Logger* logger = new Logger(file_path);
	logger->log_info(Logger::vcat("PE startet, logging at path %s", file_path));


	vector<Process_data*> procs = Process_operations::get_procs(logger,true,true); // retrieve processes

	int selected_line = 0,start=0;
	bool zero_pressed = false;
	while (!zero_pressed)	//main loop
	{
		
		if (_kbhit())
		{
			unsigned int key = _getch();
			logger->vlog_info("key %u press detected\n", key);
			switch (key)
			{
				case '1':
				{
					if (start + MAX_PAGE_SIZE < procs.size())
						start += MAX_PAGE_SIZE;

					else
						start = procs.size()-MAX_PAGE_SIZE;
					selected_line = start;
				}
				
				break;
				case '2':
				{
					if (start - MAX_PAGE_SIZE > 0)
						start -= MAX_PAGE_SIZE;
					else
						start = 0;
					selected_line = start;
				}
				break;
				case 224: //arrow keys
				{
					int key2 = _getch();
					logger->vlog_info("secondery key %d",key2);
					switch (key2)
					{
					case 72: //arrow up
					{
						if (selected_line == start)
						{
							if (start - MAX_PAGE_SIZE > 0)
								start -= MAX_PAGE_SIZE;
							else
								start = 0;
						}
						if (selected_line > 0)
						{
							selected_line--;
						}
					

					}
					
					
						break;
					case 80://arrow down
					{
						if (selected_line < procs.size() - 1)
						{
							selected_line++;
						}
						if (selected_line == start + MAX_PAGE_SIZE)
						{
							if (start + MAX_PAGE_SIZE < procs.size())
								start += MAX_PAGE_SIZE;

							else
								start = procs.size();
						}
					}
					break;
					case 75: //arrow left
					{
						if (start + MAX_PAGE_SIZE < procs.size())
							start += MAX_PAGE_SIZE;

						else
							start = procs.size() - MAX_PAGE_SIZE;
						selected_line = start;
					}
					break;
					case 77: //arrow right
					{
						if (start - MAX_PAGE_SIZE > 0)
							start -= MAX_PAGE_SIZE;
						else
							start = 0;
						selected_line = start;
					}
					
					}
				}
				break;
				case 13: //enter
				{

					DWORD pid = procs[selected_line]->pid;
					inject_dll(dll_path, pid, logger, &gui);
				}
				break;
				case '0':
				{
					zero_pressed = true;
				}

			}
			display_procs(procs, gui, start, selected_line);

			
		}

		Sleep(10);
	}
	gui.close();
	return 0;
}