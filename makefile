
CXX = clang++
CXXFLAGS = -std=c++20 -Wall -MMD -g
EXEC = main
SRCDIR = src
OBJDIR = out
CCFILES = $(wildcard $(SRCDIR)/*.cc) $(wildcard $(SRCDIR)/*/*.cc) $(wildcard $(SRCDIR)/*/*/*.cc)
OBJS = $(patsubst $(SRCDIR)/%.cc, $(OBJDIR)/%.o, $(CCFILES))
DEPS = $(patsubst $(SRCDIR)/%.cc, $(OBJDIR)/%.d, $(CCFILES))

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC)

-include $(DEPS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY:	clean

clean:
	rm -rf $(OBJDIR) $(EXEC)
