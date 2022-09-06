#pragma once
#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <algorithm>
#include <vector>

#include "Logger.h"
#include "Process_data.h"



#define ERR_NO_PROC (Process_data**)-1
#define ERR_CANOT_OPEN (Process_data*)-2
#define ERR_CANOT_GET_EXE (Process_data**)-3
#define ERR_CANOT_GET_NAME (Process_data**)-4
class Process_operations
{

public:
	static constexpr int MAX_SIZE = 1024;

	static vector<Process_data*> get_procs(Logger* logger, bool logging_flag=false, bool group_flag = false);
	static Process_data* get_proc_details(DWORD pid, Logger* logger, bool logging_flag=false);
	static int inject_dll(DWORD pid, char* dll, Logger* logger);

};

