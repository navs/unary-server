#ifndef	__NULL_LOGGER_H__
#define __NULL_LOGGER_H__

class null_logger {
public:
	static null_logger instance() { return null_logger(); }

	void fatal(...) { }
	void error(...) { }
	void info(...)  { }
	void debug(...) { }
};

#endif // __NULL_LOGGER_H__
