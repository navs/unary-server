#ifndef	__SEQUENCE_H__
#define	__SEQUENCE_H__

template<template<class> class Lock, class Mutex>
class sequence {
public:
	static int next() {
		static sequence<Lock,Mutex> seq;
		return seq.next_value();
	}
private:
	sequence() : m_seq(0) { }
	sequence(const sequence&);
	sequence& operator=(const sequence&);

	int next_value() {
		Lock<Mutex> guard(m_mtx);
		return m_seq++;
	}

	int m_seq;
	Mutex m_mtx;
};

#endif //	__SEQUENCE_H__

