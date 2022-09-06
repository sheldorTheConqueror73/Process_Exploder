#include "Process_data.h"

Process_data::Process_data()
{
	this->pid = -1;
	this->parent_pid = -1;
}
Process_data::Process_data(DWORD pid, ULONG_PTR parent_pid, char name[MAX_PATH], char path[MAX_PATH])
{
	this->pid = pid;
	this->parent_pid = parent_pid;
	memcpy_s(this->name, MAX_PATH, name, MAX_PATH);
	memcpy_s(this->path, MAX_PATH, path, MAX_PATH);
}
