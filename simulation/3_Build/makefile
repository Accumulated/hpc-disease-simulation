#Use @ to supress printing command executed

# Define target dependent flags. It defaults to G++ for cpp code.
CC=mpic++

CFLAGS=-g -O0 

ProjectName = Sim

#------------- Start of Software make related variables -------------#

# Define the needed directories of Source files, include files
# assembly files, object output directory, object dump directory
# for information about the outptu object files for further
# inspection, if needed.
SRC_DIR = ../1_Software/

# Get all include files starting from the current location of 
# make file. This assumes that the make file is in the main
# directory of the project.
#INCLUDE = ../1_Software/
INCLUDE = ../

# Object directory to dump all the output object files in this
# location for the linker to have direct access to them.
OBJ_DIR = Obj
OBJ_DIR_TMP = Objtmp

# Get all available source files from within the Src directory
SRC = $(shell find $(SRC_DIR) -name '*.cpp')

# Get all include files, make sure to modify this if the project 
# structure varies with time.
# BUG: If a single directory has multiple .h files, the variable
# will have multiple include directories of same directory. (Solve Later)
INCLUDE_DIR = $(addprefix -I , $(dir $(shell find $(INCLUDE) -name '*.h')))


# Output for preprocessing only if needed by rule PRE_PROCESS_DIR
PRE_PROCESS_DIR = PreProc


# Execution output directory
EXEC_DIR = exe

#------------- End of Software make related variables -------------#



#------------- Star of UnitTest make related variables -------------#

# Define a unit testing directory to test 
UNIT_TEST_DIR = ../2_Testing

# Get all available UT source files from within the Src directory
UT_SRC = $(shell find $(UNIT_TEST_DIR) -name '*.cpp')

#------------- End of UnitTest make related variables -------------#


# Start the build by invoking the Project exec rule.
build: $(ProjectName)
	@echo "5. Build Done"


UnitTest: Announce_UT_START TestRule
	@echo "--------------------- UniTest build done ---------------------"
	@$(shell mv $(OBJ_DIR_TMP)/main.o $(OBJ_DIR)/) 
	@$(shell rm -rf $(OBJ_DIR_TMP)) 


# Duplicate main - one from applicatoin and the other is from Unittest.
# Temporary solution is to copy application's main into a tmp directory
# and then get it back from the tmp directory. 		
Announce_UT_START:
	@echo "--------------------- UniTest build start ---------------------"
	@echo "WARNING: BUILD THE SW BEFORE RUNNING THIS RULE - UNEXPECTED BEHAVIOR"
	@$(shell mkdir $(OBJ_DIR_TMP) > /dev/null 2>&1)
	@$(shell mv $(OBJ_DIR)/main.o $(OBJ_DIR_TMP)/) 


TestRule: $(ProjectName)UT
	@echo "3. UT build done successfully"


PRE_PROCESS_ONLY: PreProcess
	@echo "Preprocessing rule Finished successfully"


PRE_PROCESS_ONLY_UT: PreProcess_UT
	@echo "Preprocessing rule Finished successfully"


# Start the all build by invoking the .bin rule
all rebuild: clean_all $(ProjectName)
	@echo "5. ReBuild Done"


$(ProjectName): SRC_RULE
	@$(CC) $(wildcard $(OBJ_DIR)/*.o) -o $(EXEC_DIR)/$@ -Wl,-Map=$(EXEC_DIR)/Map_File.map
	@echo "3. Exe rule - Finished"


$(ProjectName)UT: UT_SRC_RULE
	@$(CC) $(wildcard $(OBJ_DIR)/*.o) -o $(EXEC_DIR)/$@ -Wl,-Map=$(EXEC_DIR)/Map_FileUT.map
	@echo "2. Exe rule - Finished"


# Source files rule:
SRC_RULE: $(foreach SRC_FILE, $(SRC), $(SRC_FILE:.cpp=.o))
	@echo "2. Source compilation rule - Finished"

# UnitTest Source files rule:
UT_SRC_RULE: $(foreach SRC_FILE, $(UT_SRC), $(SRC_FILE:.cpp=.o))
	@echo "1. Source compilation rule - Finished"

PreProcess_UT: $(foreach SRC_FILE, $(UT_SRC), $(SRC_FILE:.cpp=.i))
	@echo "Source Preprocessing only - Finished"

PreProcess: $(foreach SRC_FILE, $(SRC), $(SRC_FILE:.cpp=.i))
	@echo "Source Preprocessing only - Finished"


# Rule to compile a source file to an object file
%.o: %.cpp
	@echo "Compiling CPP Code: $<"
	@$(CC) $(CFLAGS) -c $(INCLUDE_DIR) $< -o $(OBJ_DIR)/$(basename $(notdir $@)).o


# Rule to compile a source file to an object file
%.i: %.cpp
	@echo "Preprocessing file: $<"
	@$(CC) $(CFLAGS) -c $(INCLUDE_DIR) -E $< -o $(PRE_PROCESS_DIR)/$(basename $(notdir $@)).i

	
# Clean all files by removing directories of executable and objects.
# make these directories again by mkdir command and direct the result
# of this operation to null (Both stdout and stderror to be redirected
# to null. (Design choise)
.PHONY: clean_all
clean_all:
	@$(shell rm -rf $(OBJ_DIR)/* $(EXEC_DIR)/* $(PRE_PROCESS_DIR)/* $(OBJ_DIR_TMP)/*)
	@$(shell mkdir $(EXEC_DIR) $(OBJ_DIR) $(PRE_PROCESS_DIR) > /dev/null 2>&1)
	@echo "All exe and obj files are deleted"
