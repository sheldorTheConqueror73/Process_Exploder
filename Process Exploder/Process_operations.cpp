#include "Process_operations.h"


using namespace std;




vector<Process_data*> Process_operations::get_procs(Logger* logger, bool logging_flag,bool group_flag)
{
	vector<Process_data*> open_procs;
	DWORD proc_array[MAX_SIZE], cbNeeded, proc_counter;

	if (!EnumProcesses(proc_array, sizeof(proc_array), &cbNeeded))
	{
		
		cerr << "Error getting processes\n Erronum: " << GetLastError() << endl;
		return open_procs;
	}

	proc_counter = cbNeeded / sizeof(DWORD);
	
	for (int i = 0; i < proc_counter; i++)
	{
		if (logging_flag) 
		{
			logger->log_info(Logger::vcat("getting proc number %d data\n"));
			//cout <<"getting proc number "<< proc_array[i]<<" data\n";

		}
		Process_data* temp = get_proc_details(proc_array[i], logger, true);
		if(temp)
			open_procs.push_back(temp);
	/*
		if (open_procs[i] != (Process_data)ERR_CANOT_OPEN)
		{
			logger->log_info(Logger::vcat("PID: %d\t Name: %s\tPath: %s\tPPID: %llu\n", open_procs[i]->pid, open_procs[i]->name, open_procs[i]->path,open_procs[i]->parent_pid));
		//cout << "PID: " << open_procs[i]->pid << "\t Name: " << open_procs[i]->name << "\t Path: " << open_procs[i]->path << endl;

		}
	*/
	}

	if (group_flag)
	{
		sort(open_procs.begin(), open_procs.end(), [](Process_data *x, Process_data *y)->bool {return  strcmp(x->name, y->name) > 0; });
	}


	return open_procs;
}


Process_data* Process_operations::get_proc_details(DWORD pid, Logger* logger, bool logging_flag)
{

	

	CHAR executable[MAX_PATH], name[MAX_PATH];
	DWORD parent_pid;


	if (pid == 0)
	{
		memcpy_s(name, MAX_PATH, "system", sizeof("system\0"));
		memcpy_s(executable, MAX_PATH, "system", sizeof("system\0"));
	}
	else
	{
		HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
		if (!handle)
		{
			logger->log_err(Logger::vcat("failed to open process %d\tErrornum: %d\n", pid, GetLastError()));
			//cerr << "failed to open process " << pid << "\nErronum: " << GetLastError() << endl;
			return NULL;
		}
		

		

		if (!GetModuleFileNameExA(handle, NULL, executable, MAX_PATH))
		{
			logger->log_err(Logger::vcat("failed to obtain process %d path\tErrornum: %d\n", pid, GetLastError()));
			//cerr << "failed to obtain process " << pid << "path\nErronum: " << GetLastError() << endl;
			//return ERR_CANOT_GET_EXE;
		}

		if (!GetModuleBaseNameA(handle, NULL, name, sizeof(name) / sizeof(char)))
		{
			logger->log_err(Logger::vcat("failed to obtain process %d name\tErrornum: %d\n", pid, GetLastError()));
			//cerr << "failed to obtain process name" << pid << " name\nErronum: " << GetLastError() << endl;
			//return ERR_CANOT_GET_NAME;
		}

	}
	
	return new Process_data(pid,0 ,name, executable);
	
}


int Process_operations::inject_dll(DWORD pid, char* dll, Logger* logger)
{
	if (!pid)
	{
		return false;
	}
	HANDLE proc, thread;
	proc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	
	if (!proc || proc == INVALID_HANDLE_VALUE)
	{
		logger->vlog_err("failed to open process %d\n", pid);
		return false;
	}
	logger->vlog_info("opened process %d with handle %d", pid, proc);

	void* addr = VirtualAllocEx(proc, NULL, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	logger->vlog_info("memory allocated at address %d", addr);

	WriteProcessMemory(proc, addr, dll, strlen(dll) + 1, NULL);
	thread = CreateRemoteThread(proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, addr, NULL, NULL);
	if (!thread)
		return NULL;
	DWORD tid = GetThreadId(thread);
	logger->vlog_info("thread %d started with handle %d", tid, thread);

	return tid;
}