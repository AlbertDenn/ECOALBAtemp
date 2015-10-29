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

test:  test.C eunits.H fetchByCode.H fetchByRif.H DownRiver.H
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

