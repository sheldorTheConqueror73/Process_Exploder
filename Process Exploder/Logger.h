#pragma once
#include <fstream>
#include <iostream>
#include <stdarg.h>

enum Log_priority {
	info,critical,error
};

class Logger
{
	FILE *fptr;
public:
	~Logger();
	Logger(char* output_path);

	void log(Log_priority, const char* message);
	void vlog(Log_priority, const char* format, ...);
	void log_err(const char* message);
	void log_crit(const char* message);
	void log_info(const char* message);
	void vlog_info(const char* format, ...);
	void vlog_err(const char* format, ...);
	void vlog_crit(const char* format, ...);


	static const char* vcat(const char* format, ...);

};

