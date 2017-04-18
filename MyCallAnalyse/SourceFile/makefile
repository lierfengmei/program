edit:main.o Decode.o Comm_Balise_ts.o
	g++ -o edit main.o Decode.o Comm_Balise_ts.o

main.o:main.cpp Decode.h Comm_Balise_ts.h 
	g++ -c main.cpp

Decode.o:Decode.cpp Decode.h Const.h Comm_Balise_ts.h
	g++ -c Decode.cpp

Comm_Balise_ts.o:Comm_Balise_ts.cpp Comm_Balise_ts.h
	g++ -c Comm_Balise_ts.cpp

clean:
	rm edit main.o Decode.o Comm_Balise_ts.o

