#Analyse
Analyse:main.o Comm_Balise_cs.o
g++ main.o Comm_Balise_cs.o -o Analyse
main.o:main.cpp
g++ -c main.cpp -o main.o
Comm_Balise_cs.o:Comm_Balise_cs.cpp
g++ -c Comm_Balise_cs.cpp -o Comm_Balise_cs.o
