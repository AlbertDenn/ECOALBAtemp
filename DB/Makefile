PQ = /usr/include/postgresql
ALEPH = ~/aleph
DB = ../DB

CXX = g++ -O3 -g 

AR = ar

LIBNAME = DbAccess

LOCALLIB = lib$(LIBNAME).a

WARN = -Wall -Wextra -Wcast-align -Wno-sign-compare -Wno-write-strings \
       -Wno-parentheses -Wno-unused-function

FLAGS = -std=c++0x -DDEBUG -D_GLIBCXX__PTHREADS $(WARN)

INCLUDE = -I . -I $(PQ)

SOURCES = dbProperties.C dbConnection.C dbQuery.C strQuery.C

OBJECTS = dbProperties.o dbConnection.o dbQuery.o strQuery.o

lib:
	$(RM) $(LOCALLIB)
	$(CXX) $(INCLUDE) -c $(SOURCES) $(FLAGS) -fPIC
	$(AR) -cvq $(LOCALLIB) $(OBJECTS)

clean:
	$(RM) *~ *.o
clean-all:
	$(RM) *~ *.o $(LOCALLIB)

