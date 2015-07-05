APP             := led_server
CC              := g++
LD              := g++
BIN_DIR         := bin/
SOURCE_DIR      := src/
INCLUDE_DIR     := include/
SEARCH_WILDCARS := $(addsuffix /*.cpp, $(SOURCE_DIR))
SOURCE_FILES    := $(wildcard $(SEARCH_WILDCARS))
OBJECT_FILES    := $(notdir $(SOURCE_FILES))
OBJECT_FILES    := $(OBJECT_FILES:.cpp=.o)
OBJECT_FILES    := $(addprefix $(BIN_DIR), $(OBJECT_FILES))
INCLUDE_FILES   := $(wildcard $(INCLUDE_DIR)/*.h)

CFLAGS  += -I$(INCLUDE_DIR) -MD -Wall -std=c++11
LDFLAGS += -lpthread -lm

VPATH := $(SOURCE_DIR)
include $(wildcard $(BINDIR)/*.d)

$(APP): $(OBJECT_FILES)
	@echo " (LINK)   " $(OBJECT_FILES) "=>" $@
	@$(LD) $^ $(LDFLAGS) -o $@
	@echo " (OK)"

$(BIN_DIR)%.o: %.cpp $(INCLUDE_FILES)
	@mkdir -p ${BIN_DIR}
	@echo " (COMPILE)" $< "=>" $@
	@$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@rm -f *.d
	@rm -f $(APP) $(OBJECT_FILES) *.o core
	@echo " (CLEAN)  " $(APP)
