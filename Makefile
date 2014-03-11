all: csv2cpp

csv2cpp: *.h *.cpp
	g++ -o$@ *.cpp

clean:
	rm -f csv2cpp
