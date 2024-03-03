CC := gcc 
SRCDIR := src
BUILDDIR := build
TARGET := crank
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -fpic
LIB := -L lib -lm
INC := -I include

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB) -O3

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo " Building..."
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $< -save-temps -O3

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

lib: $(OBJECTS)
	$(CC) -shared -o build/stem.so build/stem.o build/better_string.o build/macros.o

install: $(TARGET) build/stem.so
	cp -r include/ /usr/include/stemlib
	cp $(TARGET) /usr/local/bin/
	cp build/stem.so /usr/local/lib/
	mkdir -p /usr/local/share/stem/
	cp -r stemlib/ /usr/local/share/stem/

site:
	doxygen
	rsync -uvrP --delete-after "html/" root@nullring.xyz:/var/www/stemdoc
