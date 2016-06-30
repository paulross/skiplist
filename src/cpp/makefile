# Makefile
# Usage:
# make help
# make depend
# make all
# make release
# make debug
PROGRAM_RELEASE = SkipList_R.exe
PROGRAM_DEBUG = SkipList_D.exe

CPP_FILES := main.cpp SkipList.cpp

OBJS := $(patsubst %.c, %.o, $(CPP_FILES))
INCLUDES = 
CC = g++
CFLAGS = -Wall -Wextra -Werror -Wfatal-errors
CFLAGS_RELEASE = $(CFLAGS) -O3 -DNDEBUG
CFLAGS_DEBUG = $(CFLAGS) -g3 -O0 -DDEBUG=1
LDFLAGS = 

all: $(PROGRAM_RELEASE) $(PROGRAM_DEBUG)

release: $(PROGRAM_RELEASE)
	@echo "Release binaries built."

$(PROGRAM_RELEASE): .depend $(OBJS)
	$(CC) $(CFLAGS_RELEASE) $(OBJS) $(LDFLAGS) -o $(PROGRAM_RELEASE)

debug: $(PROGRAM_DEBUG)
	@echo "Debug binaries built."

$(PROGRAM_DEBUG): .depend $(OBJS)
	$(CC) $(CFLAGS_DEBUG) $(OBJS) $(LDFLAGS) -o $(PROGRAM_DEBUG)

# Create file of dependancies
depend: .depend
.depend: cmd = gcc $(INCLUDES) -MM -MF depend $(VAR); cat depend >> .depend
.depend:
	@echo "Creating dependencies..."
	@$(foreach var, $(C_FILES), $(cmd))
	@rm -f depend
	@echo "Creating dependencies... DONE"

-include: depend

# These are the pattern matching rules. In addition to the automatic
# variables used here, the variable $* that matches whatever % stands for
# can be useful in special cases.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f .depend *.o src/*.o $(PROGRAM_RELEASE) $(PROGRAM_DEBUG)

.PHONY: clean depend

