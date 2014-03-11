all: csv2cpp

csv2cpp: *.h *.cpp
	g++ -g -Wall -o$@ *.cpp

clean:
	rm -f csv2cpp
