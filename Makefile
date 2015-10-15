# coloca aqui tu camino hacia aleph
ALEPH = /home/cenditel/aleph
DB = /home/cenditel/ECOALBA/testGood/DB
PQ = /usr/include/postgresql


CXX = /usr/bin/clang++-3.6


OPTIMIZATION = -O0

WARN = -Wall -Wextra -Wcast-align -Wno-sign-compare -Wno-write-strings \
       -Wno-parentheses 

FLAGS = -DDEBUG -D_GLIBCXX__PTHREADS -g $(OPTIMIZATION) -std=c++14 $(WARN)

INCLUDE = -I $(ALEPH) -I . -I $(DB) -I $(PQ) 

LIBS = -L $(ALEPH) -L $(DB) -L $(PQ)  -lAleph -lpq -lDbAccess -lnana -lc -lm \
                                     -lgsl -lgslcblas -lpthread -lgmp -lmpfr 


all: test

clean:
	rm -f test *.o

test: test.o product.o good.o grafo.o
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)  

test.o: test.C
	$(CXX) $(FLAGS) $(INCLUDE) -c test.C 

product.o: product.C types.H includes.H 
	$(CXX) $(FLAGS) $(INCLUDE) -c product.C 

good.o: good.C types.H includes.H
	$(CXX) $(FLAGS) $(INCLUDE) -c good.C

grafo.o: grafo.H grafo.C tablas.H
	$(CXX) $(FLAGS) $(INCLUDE) -c grafo.C

