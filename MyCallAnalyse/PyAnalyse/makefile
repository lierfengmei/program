edit:main.o Comm_Balise_ts.o
	g++ -o edit main.o Comm_Balise_ts.o

main.o:main.cpp Comm_Balise_ts.h 
	g++ -c main.cpp

Comm_Balise_ts.o:Comm_Balise_ts.cpp Comm_Balise_ts.h
	g++ -c Comm_Balise_ts.cpp

clean:
	rm edit main.o Comm_Balise_ts.o

