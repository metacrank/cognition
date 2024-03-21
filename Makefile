CC := gcc 
SRCDIR := src
BUILTINS_SRCDIR := src/builtins
BUILDDIR := build
BUILTINS_BUILDDIR := $(BUILDDIR)/builtins
TARGET := crank
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) $(BUILTINS_SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -fpic
LIB := -L lib -lm
INC := -I include
DESTDIR := /usr/local/bin/
$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB) -O3

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo " Building..."
	@mkdir -p $(BUILDDIR) $(BUILTINS_BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $< -save-temps -O3

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

site:
	doxygen
	rsync -uvrP --delete-after "html/" root@nullring.xyz:/var/www/stemdoc
install:
	cp crank "$(DESTDIR)"
