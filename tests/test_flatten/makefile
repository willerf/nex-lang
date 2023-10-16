
CXX = clang++
CXXFLAGS = -std=c++20 -Wall -MMD -g
EXEC = main
SRCDIR = ../../src
OBJDIR = out

# compile with all .cc files in src directory and subdirectories but exclude main.cc
CCFILES = $(filter-out $(SRCDIR)/main.cc, $(wildcard $(SRCDIR)/*.cc) $(wildcard $(SRCDIR)/*/*.cc) $(wildcard $(SRCDIR)/*/*/*.cc) $(wildcard $(*.cc)))
OBJS = $(patsubst $(SRCDIR)/%.cc, $(OBJDIR)/%.o, $(CCFILES))
DEPS = $(patsubst $(SRCDIR)/%.cc, $(OBJDIR)/%.d, $(CCFILES))

$(EXEC): $(OBJS) $(EXEC).cc
	$(CXX) $(CXXFLAGS) $(OBJS) $(EXEC).cc -o $(EXEC)

-include $(DEPS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY:	clean

clean:
	rm -rf $(OBJDIR) $(EXEC)
