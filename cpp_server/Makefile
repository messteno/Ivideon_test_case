APP 			:= led_server
CC              := g++
LD              := g++
SOURCE_DIR      := src
BIN_DIR 		:= bin/
SEARCH_WILDCARS	:= $(addsuffix /*.cpp, $(SOURCE_DIR))
SOURCE_FILES	:= $(wildcard $(SEARCH_WILDCARS))
OBJECT_FILES	:= $(notdir $(SOURCE_FILES))
OBJECT_FILES	:= $(OBJECT_FILES:.cpp=.o)
OBJECT_FILES    := $(addprefix $(BIN_DIR), $(OBJECT_FILES))
INCLUDE_DIR 	:= include/
INCLUDE_FILES   := $(wildcard $(INCLUDE_DIR)/*.h)

CFLAGS	+= -I$(INCLUDE_DIR) -MD -Wall -std=c++11
LDFLAGS += -lpthread -lm

$(APP): $(OBJECT_FILES)
	@echo " (LINK)   " $(OBJECT_FILES) "=>" $@
	@$(LD) $^ $(LDFLAGS) -o $@
	@echo " (OK)"

$(BIN_DIR)%.o: %.cpp $(INCLUDE_FILES)
	@echo " (COMPILE)" $< "=>" $@
	@$(CC) $(CFLAGS) -o $@ -c $<

VPATH := $(SOURCE_DIR)
include $(wildcard $(BINDIR)/*.d)

clean:
	@rm -f *.d
	@rm -f $(APP) $(OBJECT_FILES) *.o core
	@echo " (CLEAN)  " $(APP)
