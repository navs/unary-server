#ifndef __UTILS_H__
#define __UTILS_H__

#include <time.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <utility>
#include <iostream>

//
// Utitiles 
//
//		author:	KIM Hyuntak
//		date:	2008-12-31
//

int daemonize() {
#ifdef Linux
	pid_t pid = fork();
	if (pid < 0) {
		std::cerr << "fork() error.\n";
	}
	if (pid == 0) {
		// child
		setsid();
		std::cout << "Daemon started...\n";
	}
	// parend
	return static_cast<int>(pid);
#else 
	return 1;
#endif
}

std::string ltrim(const std::string &str)
{
	std::string::size_type pos = str.find_first_not_of(" \t\n\r");
	if (pos == std::string::npos)
	{
		return "";
	}
	return str.substr(pos);
}

std::string trim(const std::string& s)
{
	std::string::size_type pos1 = s.find_first_not_of(" \t\r\n");
    if (pos1 == std::string::npos)
    {
        return "";
    }
    std::string s2 = s.substr(pos1);
    std::string::size_type pos2 = s.find_last_not_of(" \t\r\n");
    return s2.substr(0,pos2-pos1+1);
}

std::vector<std::string> split(const std::string &str, const std::string &delim)
{
	std::vector<std::string> vs;
	std::string s = str;
	std::string::size_type pos;
	while ((pos = s.find_first_of(delim)) != std::string::npos)
	{
		if (pos > 0)
		{
			vs.push_back(ltrim(s.substr(0, pos)));
		}
		if (pos < str.length() - 1)
		{
			s = s.substr(pos+1);
		}
		else
		{
			break;
		}
	}
	if (!s.empty())
	{
		vs.push_back(ltrim(s));
	}
	return vs;
}

std::pair<std::string,std::string> split_to_pair(
	const std::string& s, const std::string& delim)
{
    std::string first = "";
    std::string second = "";
	std::string::size_type pos = s.find_first_of(delim);
    if (pos == std::string::npos)
    {
        return std::pair<std::string, std::string>(trim(s),"");
    }
    first = trim(s.substr(0, pos));
    second = trim(s.substr(pos+1));
    return std::pair<std::string, std::string>(first, second);
}

//
// return timestamp
//
//	FIXME: using fmt ...
std::string timestamp(const std::string& fmt = "")
{
	time_t now = time(NULL);
	struct tm t;
#ifdef Linux
	localtime_r(&now, &t);
#elif defined(WIN32)
	localtime_s(&t, &now);
#endif
	char buffer[256];
	sprintf_s(buffer, "%04d%02d%02d.%02d%02d%02d",
		t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	return std::string(buffer);
}

#endif //  __UTILS_H__
