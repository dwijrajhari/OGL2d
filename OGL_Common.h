#include <iostream>
enum Log { WM_INFO, WM_WARNING, WM_ERROR };

template <typename T> void fLog(int pLogLevel, T pMsg)
{
	std::cout << "[" << pLogLevel << "]:: " << pMsg << std::endl;
}