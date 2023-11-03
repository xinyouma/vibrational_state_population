# ------------------------------------------------
# Generic Makefile
#
# ------------------------------------------------

# project name (generate executable with this name)
TARGET   = vib_state_pop.x

CC       = c++
# compiling flags here
#CFLAGS   = -std=c++11 -Wall -I.
CFLAGS   = -Wall -I.
CFLAGS   += -std=c++11

LINKER   = c++
# linking flags here
LFLAGS   = -Wall -I. -lm

# change these to proper directories where each file should be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.hpp)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
rm       = rm -f


$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: remove
remove:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: clean
clean: remove
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
