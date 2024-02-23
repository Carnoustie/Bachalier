CXX := g++
CXXFLAGS := -I. $(shell pkg-config gtkmm-3.0 giomm-2.4 --cflags) -O
LDFLAGS := $(shell pkg-config gtkmm-3.0 giomm-2.4 --libs)

SRC := SyntheticData.cpp Plot.cpp
OBJ := $(SRC:.cpp=.o)
DEP := $(OBJ:.o=.d)  # Dependency files for each object
MAIN := s

.PHONY: all clean

all: $(MAIN)

$(MAIN): $(OBJ)
	$(CXX) -o $(MAIN) $(OBJ) $(LDFLAGS)

# Rule to compile C++ files into object files
# Also generates a .d file containing the header dependencies
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

# Include the dependency files
-include $(DEP)

clean:
	rm -f $(MAIN) $(OBJ) $(DEP)
