EXEC=mazegen
FILES=src/main.c src/images.c src/maze.c
all:
	# Compiler is ${CC}.
	${CC} -g -Wall -Wpedantic -lpng ${FILES} -o ${EXEC}
