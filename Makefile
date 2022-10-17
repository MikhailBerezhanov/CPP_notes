CXX=g++
CC=gcc
CXXFLAGS= -std=c++11
#CFLAGS=-Wall -Wno-format-security -Wno-unused-variable -Wno-unused-function

# Default value for application home path

MAIN_DIR = .
BIN_NAME = tests.out

# PATHS
BIN_DIR = $(MAIN_DIR)/output
OBJ_DIR = $(MAIN_DIR)/obj
SRC_DIR = $(MAIN_DIR)/lessons

INC_PATHS = $(SRC_DIR)
INCLUDE_PREFIX = -I
INCLUDES = $(addprefix $(INCLUDE_PREFIX), $(INC_PATHS))

LIBS_INC_PREFIX = -L
LIBS =
LIBS_INC = $(addprefix $(LIBS_INC_PREFIX ), $(LIBS))

LINK_LIBS = -lpthread

DEFINE_PREFIX = -D   
DEFINES =

SRC_FILES=	$(SRC_DIR)/main.cpp \
			$(SRC_DIR)/Sales_data.cpp \
			$(SRC_DIR)/3_str_vec_arr.cpp \
			$(SRC_DIR)/4_expressions.cpp \
			$(SRC_DIR)/5_operators.cpp \
			$(SRC_DIR)/6_functions.cpp \
			$(SRC_DIR)/7_classes.cpp \
			$(SRC_DIR)/8_iostreams.cpp \
			$(SRC_DIR)/9_serial_containers.cpp \
			$(SRC_DIR)/10_algorithms.cpp \
			$(SRC_DIR)/11_assoc_containers.cpp \
			$(SRC_DIR)/Str_blob.cpp \
			$(SRC_DIR)/Text_query.cpp \
			$(SRC_DIR)/12_smart_pointers.cpp \
			$(SRC_DIR)/Has_ptr.cpp \
			$(SRC_DIR)/Message.cpp \
			$(SRC_DIR)/StrVec.cpp \
			$(SRC_DIR)/13_copy_control.cpp \
			$(SRC_DIR)/MyString.cpp \
			$(SRC_DIR)/14_operators_overload.cpp \
			$(SRC_DIR)/Query.cpp \
			$(SRC_DIR)/15_OOP.cpp \
			$(SRC_DIR)/16_templates.cpp \
			$(SRC_DIR)/18_exceptions_namespaces.cpp \
			$(SRC_DIR)/18_multi_inheritance.cpp \
			$(SRC_DIR)/19_misc.cpp \
			$(SRC_DIR)/threads_and_tasks.cpp \

OBJS_FILES = $(SRC_FILES:.cpp=.o)
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(OBJS_FILES)))

.PHONY : clean install

all: prep bin

# all: clean
# 	@LC_ALL=C $(CCPP) $(CFLAGS) -O0 $(DEFINES) \
# 	$(SRC_FILES) \
# 	-o $(EXECNAME) \
# 	$(INC) \
# 	$(LLIBS) 

# Prepare directories for output
prep:
	@if test ! -d $(BIN_DIR); then mkdir $(BIN_DIR); fi
	@if test ! -d $(OBJ_DIR); then mkdir $(OBJ_DIR); fi

#$(SRC_DIRS:.cpp=.o): %.o:%.cpp
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	@echo Compiling $(OBJ_DIR)/$(@F) from C++ SOURCE $<
	@$(CXX) -c $(CXXFLAGS) $(INCLUDES) $(DEFINES) $< -o $(@) 
	@echo --------------------------------------------------------

# Start linker
bin:$(OBJS)
	@echo Linking binary file: $(BIN_NAME)
	@$(CXX) -o $(BIN_DIR)/$(BIN_NAME) $^ $(LINK_LIBS)
	@echo "\033[32mBuilding finished [$(shell date +"%T")]\033[0m"

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

debug: clean
	@LC_ALL=C $(CXX) $(CXXFLAGS) -O3 -g $(DEFINES) $(MY_DEFINES) \
	$(SRC_FILES) \
	-o $(BIN_DIR)/$(BIN_NAME) \
	$(INCLUDES) \
	$(LLIBS) 

run:
	@$(BIN_DIR)/$(BIN_NAME)


mem_check: debug
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(EXECNAME)
	

