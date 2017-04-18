#edit:main.o Comm_Balise_ts.o
#	g++ -o edit main.o Comm_Balise_ts.o

libDecode.so:main.cpp Decode.cpp
	g++ -o libDecode.so -shared -fPIC main.cpp Decode.cpp

clean:
	rm libDecode.so

run:
	python Decode.py

#main.o:main.cpp Comm_Balise_ts.h 
#	g++ -c main.cpp

#Comm_Balise_ts.o:Comm_Balise_ts.cpp Comm_Balise_ts.h
#	g++ -c Comm_Balise_ts.cpp

#clean:
#	rm main.o Comm_Balise_ts.o libAnalyse.so

