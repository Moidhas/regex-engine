CXX = g++-13
CXXFLAG = -std=c++20 -Wall -g -MMD
EXEC= prog
OBJECTS= a1q1.o
DEPENDS= ${OBJECTS:.o=d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAG} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

PHONY: clean
clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}

