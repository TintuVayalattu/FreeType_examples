IDIR =/usr/include/freetype2 /usr/local/include/harfbuzz
CC = gcc
CFLAGS = $(addprefix -I,$(IDIR))

ODIR = obj
LDIR = /usr/local/lib
LIBS = -lgraph -lfreetype -lm -lharfbuzz
LFLAGS = $(addprefix -L,$(LDIR)) $(LIBS)

_DEPS =
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = harfbuzz_ex.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

harfbuzz_ex: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
