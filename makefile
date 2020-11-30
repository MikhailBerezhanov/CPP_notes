CCPP=g++
CC=gcc
CFLAGS=
#CFLAGS=-Wall -Wno-format-security -Wno-unused-variable -Wno-unused-function

# Default value for application home path
DEFINES = -DAPP_DIR=\"$(APP_DIRECTORY)\"
DEFINES += -DAPP_MULTITHREADED=1
DEFINES += -DDEBUG_LOG_FILE_SUPPORT=1

EXECNAME="a.out"

# PATHS
SRC=./
INC=-I./

SRC_FILES=	$(SRC)main.cpp \
			$(SRC)Sales_data.cpp \
			$(SRC)chapter3.cpp \
			$(SRC)chapter4.cpp \
			$(SRC)chapter5.cpp \
			$(SRC)chapter6.cpp \
			$(SRC)chapter7.cpp \
			$(SRC)chapter8.cpp \
			$(SRC)chapter9.cpp \
			$(SRC)chapter10.cpp \
			$(SRC)chapter11.cpp \
			$(SRC)Str_blob.cpp \
			$(SRC)Text_query.cpp \
			$(SRC)chapter12.cpp \
			$(SRC)Has_ptr.cpp \
			$(SRC)Message.cpp \
			$(SRC)StrVec.cpp \
			$(SRC)chapter13.cpp \
		  
LLIBS=


.PHONY : clean install

all: clean
	@LC_ALL=C $(CCPP) $(CFLAGS) -O0 $(DEFINES) \
	$(SRC_FILES) \
	-o $(EXECNAME) \
	$(INC) \
	$(LLIBS) 

debug: clean
	@LC_ALL=C $(CCPP) $(CFLAGS) -O3 -g $(DEFINES) $(MY_DEFINES) \
	$(SRC_FILES) \
	-o $(EXECNAME) \
	$(INC) \
	$(LLIBS) 

run:
	@./$(EXECNAME)

install:

mem_check: debug
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(EXECNAME)
	
service:

update:

clean:
	@rm -f ./$(EXECNAME)
