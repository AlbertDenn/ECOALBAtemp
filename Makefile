# coloca aqui tu camino hacia aleph
ALEPH = /home/cenditel/aleph
DB = /home/cenditel/ECOALBA/testGood/DB
PQ = /usr/include/postgresql
AL = /home/cenditel/ECOALBA/testGood


CXX = /usr/bin/clang++-3.7


OPTIMIZATION = -O0

WARN = -Wall -Wextra -Wcast-align -Wno-sign-compare -Wno-write-strings \
       -Wno-parentheses -Wunused-parameter

FLAGS = -DDEBUG -D_GLIBCXX__PTHREADS -g $(OPTIMIZATION) -std=c++14 $(WARN)

INCLUDE = -I $(ALEPH) -I . -I $(DB) -I $(PQ)

LIBS = -L $(ALEPH) -L $(DB) -L $(PQ)  -lAleph -lpq -lDbAccess -lnana -lc -lm \
                                     -lgsl -lgslcblas -lasprintf -lpthread -lgmp -lmpfr 


all: test

clean:
	rm -f test *.o *.csv

test:  test.o grafo.o csvparser.o 
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

test.o: test.C fetchDB.H
	$(CXX) $(FLAGS) $(INCLUDE) -c test.C

grafo.o: grafo.H grafo.C csvparser.o
	$(CXX) $(FLAGS) $(INCLUDE) -c grafo.C

csvparser.o: csvparser.h csvparser.c
	$(CC) $(INCLUDE) $*.c -g -c

