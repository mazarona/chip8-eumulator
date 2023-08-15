# Binary Utilities
CC = gcc

# Flags to compiler
OPT = -O0
DEPFLAGS = -MP -MD
CFLAGS = -c -std=gnu11 -Wall $(foreach D,$(INC_DIR),-I$(D)) $(OPT) $(DEPFLAGS) `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`


# Directories paths
INC_DIR = include
SRC_DIR = src

# File paths within the directories
SRCS = $(foreach D,$(SRC_DIR),$(wildcard $(D)/*.c))
OBJS = $(patsubst %.c,%.o,$(SRCS))
DEPS = $(patsubst %.c,%.d,$(SRCS))
BINARY = ./bin/main 

all : $(BINARY)

$(BINARY) : $(OBJS)
	$(info ==================== GENERATING Binary =======================)
	$(CC) $(LDFLAGS) $^ -o $@

%.o : %.c 
	$(info ================= GENERATING OBJECT FILES ====================)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJS) $(BINARY) $(DEPS) 


# include the dependencies
-include $(DEPS)
