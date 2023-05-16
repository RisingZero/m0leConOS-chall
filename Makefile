
GXX=gcc
GXX_FLAGS=-Wall

SRC_DIR=src
BUILD_DIR=build
DIST_DIR=dist

INCLUDE_DIR = ${SRC_DIR}/include
LIB_DIR = ${SRC_DIR}/lib

EXECUTABLE_NAME=m0leConOS

.PHONY: fslib

target: ${SRC_DIR}/main.c fslib
	${GXX} ${GXX_FLAGS} -I${INCLUDE_DIR} ${SRC_DIR}/main.c ${BUILD_DIR}/*.o -o ${DIST_DIR}/${EXECUTABLE_NAME}

release: ${SRC_DIR}/main.c fslib
	${GXX} ${GXX_FLAGS} -s -I${INCLUDE_DIR} ${SRC_DIR}/main.c ${BUILD_DIR}/*.o -o ${DIST_DIR}/${EXECUTABLE_NAME}
	strip --strip-all ${DIST_DIR}/${EXECUTABLE_NAME}

fslib: ${SRC_DIR}/fs.c ${INCLUDE_DIR}/fs.h
	${GXX} ${GXX_FLAGS} -I${INCLUDE_DIR} -c ${SRC_DIR}/fs.c -o ${BUILD_DIR}/fs.o


run: target
	./${DIST_DIR}/${EXECUTABLE_NAME}

clean: distclean
	rm -rf ${BUILD_DIR}/*

distclean:
	rm -rf ${DIST_DIR}/*