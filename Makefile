#  Universal Makefile
#   Works well almost everywhere

CC=gcc
FILETYPE=c

CCFLAG=-Wall -g 
LDFLAG=-lpulse -lpulse-simple

EXE=music-server

SRCDIR:=src
OBJDIR:=obj
BINDIR:=bin
DEPDIR:=dep

DIRS:=$(SRCDIR) $(OBJDIR) $(BINDIR) $(DEPDIR)

SRC:=$(wildcard $(SRCDIR)/*.$(FILETYPE))
OBJ:=$(patsubst $(SRCDIR)/%.$(FILETYPE), $(OBJDIR)/%.o, $(SRC))
DEP:=$(patsubst $(OBJDIR)/%.o, $(DEPDIR)/%.d, $(OBJ))

.PHONY: all clean mrproper 

all: $(DIRS) $(BINDIR)/$(EXE) 

-include $(wildcard $(DEPDIR)/*.d)

$(DIRS): 
	@mkdir -p $(DIRS)

$(BINDIR)/$(EXE): $(OBJ)
	@echo "Linking all object files together..."
	@$(CC) $^ -o $@ $(LDFLAG)
	@echo "OK!"

$(OBJDIR)/%.o: $(SRCDIR)/%.$(FILETYPE)
	@echo "Creating the $* object file..."
	@$(CC) -c -o $(OBJDIR)/$*.o $(SRCDIR)/$*.$(FILETYPE) $(CCFLAG)
	@echo "Generating the dependancy file for $*"
	@echo -n "$(OBJDIR)/" > $(DEPDIR)/$*.d
	@$(CC) -MM $(CCFLAG) $(SRCDIR)/$*.$(FILETYPE) >> $(DEPDIR)/$*.d

clean:
	@echo "Deleting temporary files..."
	@rm -f $(OBJDIR)/*.o $(DEPDIR)/*.d

mrproper:
	@echo "Deleting all unnecessary directories..."
	@rm -rf $(OBJDIR) $(BINDIR) $(DEPDIR)
