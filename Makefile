
OS=$(shell uname)

CPPFLAGS=-D$(OS) -g
LDFLAGS=-pthread

nutd:	main.cpp
	g++ -o nutd main.cpp $(CPPFLAGS) $(LDFLAGS)

clean:	
	rm -rf nutd
