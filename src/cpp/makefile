# Makefile
# Usage:
# make help
# make depend
# make all
# make release
# make debug
PROGRAM_RELEASE = SkipList_R.exe
PROGRAM_DEBUG = SkipList_D.exe

CXX_FILES := main.cpp	SkipList.cpp	test/test_documentation.cpp	test/test_functional.cpp	test/test_performance.cpp	test/test_print.cpp	test/test_rolling_median.cpp	test/test_concurrent.cpp

OBJS := $(patsubst %.c, %.o, $(CXX_FILES))
INCLUDES = -I . -I test/
#CXX = g++
override CXXFLAGS += -Wall -Wextra -Werror -Wfatal-errors -std=c++11
CXXFLAGS_RELEASE = $(CXXFLAGS) -O2 -DNDEBUG
CXXFLAGS_DEBUG = $(CXXFLAGS) -g3 -O0 -DDEBUG=1
LDFLAGS = 

all: $(PROGRAM_RELEASE) $(PROGRAM_DEBUG)

release: $(PROGRAM_RELEASE)
	@echo "Release binaries built."

$(PROGRAM_RELEASE): .depend $(OBJS)
	$(CXX) $(CXXFLAGS_RELEASE) $(OBJS) $(LDFLAGS) -o $(PROGRAM_RELEASE)

debug: $(PROGRAM_DEBUG)
	@echo "Debug binaries built."

$(PROGRAM_DEBUG): .depend $(OBJS)
	$(CXX) $(CXXFLAGS_DEBUG) $(OBJS) $(LDFLAGS) -o $(PROGRAM_DEBUG)

# Create file of dependancies
depend: .depend
.depend: cmd = $(CXX) $(INCLUDES) -MM -MF depend $(var); cat depend >> .depend; rm -f depend;
.depend:
	@echo "Creating dependencies..."
	@$(foreach var, $(CXX_FILES), $(cmd))
	@echo "Creating dependencies... DONE"

-include: depend

# These are the pattern matching rules. In addition to the automatic
# variables used here, the variable $* that matches whatever % stands for
# can be useful in special cases.
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

%: %.c
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f .depend *.o src/*.o $(PROGRAM_RELEASE) $(PROGRAM_DEBUG)

.PHONY: clean depend

print-%:
	@echo $*=$($*)
