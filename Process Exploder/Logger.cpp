#include "Logger.h"
#define ERR_CANT_OPEN_FILE -5

Logger::~Logger()
{
	fclose(fptr);
}
Logger::Logger(char* output_path)
{

	errno_t err = fopen_s(&fptr, output_path, "a+");
	//if (!fptr)
	//{
	//	printf( "Logger cannot open file errno%d\n",errno);
	//	//errcode = ERR_CANT_OPEN_FILE;
	//}
	//else
	//{
	//	printf("Logger file open at %s\n", output_path);
	//	//errcode = EXIT_SUCCESS;
	//}
}

void Logger::log(Log_priority priority, const char* message)
{
	char tag[30];
	switch (priority)
	{
	case Log_priority::critical: memcpy_s(tag, 30 * sizeof(char), "[CRITICAL]\t", sizeof("[CRITICAL]\t"));
		break;
	case Log_priority::error:  memcpy_s(tag, 30 * sizeof(char), "[ERROR]\t", sizeof("[ERROR]\t"));
		break;
	case Log_priority::info:  memcpy_s(tag, 30 * sizeof(char), "[INFO]\t", sizeof("[INFO]\t"));

	}
	fprintf(fptr, "%s%s", tag, message);
}

void  Logger::vlog(Log_priority priority, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	this->log(priority, vcat(format, ap));
	va_end(ap);
}

void Logger::log_err(const char* message)
{
	this->log(Log_priority::error, message);
}
void Logger::log_crit(const char* message)
{
	this->log(Log_priority::critical, message);
}
void Logger::log_info(const char* message)
{
	this->log(Log_priority::info, message);
}
void  Logger::vlog_info(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	this->vlog(Log_priority::info, format, ap);
	va_end(ap);
}
void  Logger::vlog_err(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	this->vlog(Log_priority::error, format, ap);
	va_end(ap);
}
void  Logger::vlog_crit(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	this->vlog(Log_priority::critical, format, ap);
	va_end(ap);
}



const char* Logger::vcat(const char* format, ...)
{
	char* buffer = new char[101];
	va_list ap;
	va_start(ap, format);
	vsnprintf(buffer, 100, format, ap);
	va_end(ap);

	return buffer;
}