# to write comments ##
##################################### Makefile ###############################
# a Makefile works accordingly to the following syntax:
#
# rule: dependencies
# 	<TAB>	comand1
# 	<TAB>	comand2
#
# all: the name of the rules to be executed
# teste.o: teste.c (can be interpreted as destination_file: origin_file)
# clean: deletes the intermediary files. If one writes in the terminal <make
# clean> the object files are deleted.
# mrproper: deletes everything that must be modified
#
# ----- Defining variables -----------------------------------
#  VAR_NAME=VAL
#  and to use the variable we placed between $(): $(VAR_NAME) 
#
#	#Using wildcards to help define variables
#	SRC=$(wildcard *.c)
#	OBJ=$(SRC:.c=.o)
# -------------------------------------------------------------
# 
# #Silent Mode (no echo in the command line)
# add @ to the beginning of the command. Ex:
# 				@$(CC) -o $@ $^
#
# ------------------- Internal Variables ---------------------
# $@	Name of the rule
# $<	Name of the 1st dependency
# $^	List of dependencies
# $?	List of dependencies more recent than the rule
# $*	Name of the file with suffix
# ------------------------------------------------------------
# 
#	Rules without dependencies, e.g., actions like clean:, must be called
#	explicitly, otherwise they wont be executed.
#			make clean #will remove all objects files from folder, as described below
#
# -------------------- Interference rules -------------------
#  Generic rules called by default
#  - .c.o: make a .o file from a .c file
#  - %o: %.c: the same thing. The line teste.o:teste.c can be modified with this
#  rule
#  - .PHONY: This rule avoids conflicts.
#  			- If one has a clean file in the directory, nothing will happen when one
#  			executes make clean
#  			- So .PHONY says that clean and mrproper should be executed even if
#  			files with that name exist current path
# ----------------------------------------------------------------
# 
# ------------------------- make install -------------------------
#  Automating the program's installation with the install: rule
#   - install: Places the binary or executable inside a given folder, e.g., /bin
#   or /usr/bin in Linux. It can be any other, using the command mv or cp to
#   move or copy
#   - Lets create 2 vars:
#   	- prefix=path/to/proj
#   	- bindir=$(prefix)/bin
#   	- and we add the rule install:all

# PROJ name (can be anything one wants)
PROJ=webcam-main

# File Paths
SRC_DIR=.
BIN_DIR=${SRC_DIR}/bin

#CC=clang++ #defining the compiler; bad pratice to define it (not portable)
CC=g++

# Pre-processor (CPP), Compiler (CXX) and Linker (LD) settings
CPPFLAGS = #-I '/usr/local/opt/llvm/include/c++/v1', '/usr/local/Cellar/llvm/3.5.1/include/c++/v1' # e.g. (-I, -D, etc)
#CXXFLAGS = -Wall #-Werror  # e.g (-W -ansi -pedantic)
# changed CXXFLAGS and CC because of tuple
# src: https://github.com/rogual/neovim-dot-app/issues/46#issuecomment-90966755
CXXFLAGS = -std=c++17 -Wall -ljpeg -lv4l1 -lv4l2 -DIO_READ -DIO_MMAP -DIO_USERPTR #-pthread #-Werror  # e.g (-W -ansi -pedantic)
#LDFLAGS = -lbluetooth -lpthread #-lform -lncurses

RM=rm -rf

# defining SRC and OBJ files based on wildcards
SRC := $(wildcard *.cpp)
HEAD:= $(wildcard *.h)
OBJ := $(SRC:.cpp=.o)


# BUILDING EXECUTABLE (w/ linking)
$(PROJ): $(OBJ)
# the compiler does the linking between the 2 objects
# gcc -o teste teste.o main.o
# $@ = teste:			#Rule name
# $^ = teste.o main.o		#dependency list
	@echo "Creating executable"
	$(CC) -o $@ $^ ${LDFLAGS}

# COMPILING: Building object files from sources
%.o: ${SRC}/%.cpp $(HEAD) # make a .o file from a .c file
# gcc -o teste.o -c teste.c -W -Wall -ansi -pedantic
# $< Name of the 1st dependency
	@echo "Building object files"
	$(CC) -o $@ -c $^ $(CPPFLAGS) $(CXXFLAGS)

all: $(PROJ)
	@echo "Compiling project"

# Install: run make and then make install
install: all clean
	@echo "Installing binaries"
	@mkdir -p $(BIN_DIR)
	@mv $(PROJ) $(BIN_DIR)/
	@mv $(BIN_DIR) ../

.PHONY: clean mrproper
clean: 
# @- $(RM) *.o # this does not work	
#	@- $(RM) $(wildcard *.o) # this does not work
#	this works:
	 @- $(RM) $(OBJ)
	 @- $(RM) $(PROJ)
mrproper: clean
	@$(RM) $(EXEC)
