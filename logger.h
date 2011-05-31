#ifndef	__LOGGER_H__
#define	__LOGGER_H__

#include <cstdarg>
#include <cstdio>
#include <fstream>
#include <string>
#include "utils.h"

#pragma warning(push)
#pragma warning(disable:4355)

// 
//	Logger class
//		Author:	KIM Hyuntak (navsis@gmail.com)
//		Date:	2009-01-02
//
class logger {
public:
	// Singleton
	static logger& instance() {
		static logger log;
		return log;
	}
	// FLAGS and LOGLEVEL
	static const int DAILYLOG = 0x1;
	static const int CONSOLE_LOG = 0x2;
	static const int NOFILE_LOG = 0x4;

	static const int NOLOG = 0;
	static const int FATAL = 1;
	static const int ERROR = 2;
	static const int INFO = 3;
	static const int DEBUG = 4;

	// config
	int config(	const std::string& dir,
				const std::string& prefix, 
				const std::string& fmt,
				const std::string& option) {
		m_dir = dir;
		m_prefix = prefix;
		m_fmt = fmt;
		m_flags = DAILYLOG;

		if (option.find("CONSOLE") != std::string::npos) {
			m_flags |= CONSOLE_LOG;
		}
		if (option.find("NOFILE") != std::string::npos) {
			m_flags |= NOFILE_LOG;
		}
		if (option.find("ONELOGFILE") != std::string::npos) {
			m_flags &= ~DAILYLOG;
		}
		if (option.find("FATAL") != std::string::npos) {
			level(FATAL);
		}
		if (option.find("ERROR") != std::string::npos) {
			level(ERROR);
		}
		if (option.find("INFO") != std::string::npos) {
			level(INFO);
		}
		if (option.find("DEBUG") != std::string::npos) {
			level(DEBUG);
		}
		return 0;
	}
	
	int level() const { return m_level; }
	int flags() const { return m_flags; }
	const std::string& fmt() const { return m_fmt; }
	void level(int level) { m_level = level; } void flags(int flags) { m_flags = flags; }
	void fmt(const std::string& fmt) { m_fmt = fmt; }
	const std::string filename(int level) {
		std::string name = m_dir + "/" + m_prefix;
		if (m_flags & DAILYLOG) {
			name += "_" + timestamp().substr(0,8);
		}
		name += ".log";
		return name;
	};

	const std::string prefix(int level) const {
		std::string pf = m_prefix + " " + timestamp();
		// TODO implement
		//		%d datetime;
		//		%p process id
		//		%t thread id
		//		%P prefix
		//		%L loglevel
		pf += (level == FATAL)?" [FATAL] ":
			(level == ERROR)?" [ERROR] ":
			(level == INFO)?" [INFO] ":
			(level == DEBUG)?" [DEBUG] ":" ";
		return pf;
	}
	
	bool log_to_console() const { return (m_flags & CONSOLE_LOG)!=0; }
	bool log_to_file() const { return (m_flags & NOFILE_LOG)==0; }

protected:
	template<int Level> class logwriter {
	public:
		logwriter(logger& log) : m_log(log) { }
		int operator()(const char* fmt, ...) {
			if (m_log.level() < Level) {
				return 0;
			}
			//  make log data  to write
			va_list ap;

#ifdef Linux
			va_start(ap, fmt);
			int len = std::vsnprintf(NULL, 0, fmt, ap);
			va_end(ap);
			char *str = new char[len+1];
			va_start(ap, fmt);
			len = std::vsnprintf(str, len+1, fmt, ap);
			va_end(ap);
#endif 
#ifdef WIN32
			va_start(ap, fmt);
			int len = 2048;
			char* str = new char[len+1];
			len = vsnprintf_s(str, len+1, _TRUNCATE, fmt, ap);
			va_end(ap);
#endif

			//  write to file
			if (m_log.log_to_file()) {
				std::string file = m_log.filename(Level);
				std::ofstream outfile;
				outfile.open(file.c_str(),
					std::ofstream::out | std::ofstream::app);
				if (outfile.good()) {
					outfile << m_log.prefix(Level);
					outfile << str << std::endl;
				};
				outfile.close();
			}

			//  write to console
			if (m_log.log_to_console()) {
				std::cout << m_log.prefix(Level);
				std::cout << str << std::endl;
			}

			if (str) {
				delete [] str;
			}
			return 0;
		}
	protected:
		logger& m_log;
	};
public:
	logwriter<FATAL> fatal;
	logwriter<ERROR> error;
	logwriter<INFO> info;
	logwriter<DEBUG> debug;

protected:
	logger() : fatal(*this), error(*this), info(*this), debug(*this),
				m_dir("./dir"), m_level(INFO), m_flags(DAILYLOG) { }
	logger(const logger&);
	logger& operator=(const logger&);

	int m_level;
	int m_flags;
	std::string m_dir;
	std::string m_prefix;
	std::string m_fmt;
};

#pragma warning(pop)

#endif // __LOGGER_H__
