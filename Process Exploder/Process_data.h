#pragma once
#include<Windows.h>
#include <stdio.h>

using namespace std;

class Process_data
{
public:
	DWORD pid;
	ULONG_PTR parent_pid;
	char name[MAX_PATH]="Unknown";
	char path[MAX_PATH]="Unknown";

	

	Process_data();
	Process_data(DWORD pid, ULONG_PTR parent_pid, char name[MAX_PATH], char path[MAX_PATH]);


};

