edit:main.o Decode.o
	g++ -o edit main.o Decode.o

main.o:main.cpp Decode.h
	g++ -c main.cpp

Decode.o:Decode.cpp Decode.h Const.h
	g++ -c Decode.cpp

clean:
	rm edit main.o Decode.o
