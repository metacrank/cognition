CC := gcc 
SRCDIR := src
INCDIR := include
BUILTINS_SRCDIR := src/builtins
BUILDDIR := build
BUILTINS_BUILDDIR := $(BUILDDIR)/builtins
TARGET := crank
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) $(BUILTINS_SRCDIR) -type f -name '*.$(SRCEXT)')
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -fpic -std=c11
LIB := -L lib -lm
INC := -I $(INCDIR)
DESTDIR := /usr/local/bin
INC_DESTDIR := /usr/local/include
$(TARGET): $(OBJECTS)
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB) -O3

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR) $(BUILTINS_BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $< -save-temps -O3

clean:
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

site:
	doxygen
	rsync -uvrP --delete-after "html/" root@nullring.xyz:/var/www/stemdoc

install:
	cp crank "$(DESTDIR)"
	cp -r $(INCDIR)/* $(INC_DESTDIR)

uninstall:
	rm $(DESTDIR)/crank
	rm -r $(INC_DESTDIR)/builtins

uninstall-others:
	rm $(shell find $(INC_DESTDIR) -maxdepth 1 -type f -name '*.h')
