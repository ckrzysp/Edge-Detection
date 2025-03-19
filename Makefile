########################################
##
## Makefile
## LINUX compilation 
##
##############################################





#FLAGS
C++FLAG = -g -std=c++14

MATH_LIBS = -lm

EXEC_DIR=.


.cc.o:
	g++ $(C++FLAG) $(INCLUDES)  -c $< -o $@


#Including
INCLUDES=  -I. 

#-->All libraries (without LEDA)
LIBS_ALL =  -L/usr/lib -L/usr/local/lib 

# h1
h1_OBJ = image.o h1.o
PROGRAM_NAME_h1 = h1

$(PROGRAM_NAME_h1): $(h1_OBJ)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(h1_OBJ) $(INCLUDES) $(LIBS_ALL)

# h2
h2_OBJ = image.o h2.o
PROGRAM_NAME_h2 = h2

$(PROGRAM_NAME_h2): $(h2_OBJ)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(h2_OBJ) $(INCLUDES) $(LIBS_ALL)

# h3
h3_OBJ = image.o h3.o
PROGRAM_NAME_h3 = h3

$(PROGRAM_NAME_h3): $(h3_OBJ)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(h3_OBJ) $(INCLUDES) $(LIBS_ALL)

# h4
h4_OBJ = image.o DisjSets.o h4.o
PROGRAM_NAME_h4 = h4

$(PROGRAM_NAME_h4): $(h4_OBJ)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(h4_OBJ) $(INCLUDES) $(LIBS_ALL)


all:
	make $(PROGRAM_NAME_h1) $(PROGRAM_NAME_h2) $(PROGRAM_NAME_h3) $(PROGRAM_NAME_h4)

clean:
	(rm -f *.o; rm h1 h2 h3 h4)

(:
