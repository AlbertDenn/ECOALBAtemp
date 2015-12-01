# coloca aqui tu camino hacia aleph
ALEPH = /home/cenditel/aleph
DB = /home/cenditel/sourceCodes/ECOALBA/chainsCreator/DB
PQ = /usr/include/postgresql
AL = /home/cenditel/sourceCodes/ECOALBA/chainsCreator


CXX = /usr/bin/clang++-3.7


OPTIMIZATION = -O0

WARN = -Wall -Wextra -Wcast-align -Wno-sign-compare -Wno-write-strings \
       -Wno-parentheses -Wunused-parameter

FLAGS = -DDEBUG -D_GLIBCXX__PTHREADS -g $(OPTIMIZATION) -std=c++14 $(WARN)

INCLUDE = -I $(ALEPH) -I . -I $(DB) -I $(PQ)

LIBS = -L $(ALEPH) -L $(DB) -L $(PQ)  -lAleph -lpq -lDbAccess -lnana -lc -lm \
                                     -lgsl -lgslcblas -lasprintf -lpthread -lgmp -lmpfr 


all: chainCreate

clean:
	rm -f test *.o *.csv outjson.txt chainCreate

chainCreate:  chainCreate.C eunits.H fetchRoot.H 
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

