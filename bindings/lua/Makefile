
CFLAGS += -std=c99 -Wall -O3
ALL_CFLAGS = -D_XOPEN_SOURCE=600 -fPIC -DFIU_ENABLE=1 \
		-I. -I../../libfiu/ $(CFLAGS)

CFLAGS	+= -I/usr/include/lua5.2
LDFLAGS	+= -llua5.2

# prefix for installing the binaries
PREFIX=/usr/local

# install utility, we assume it's GNU/BSD compatible
INSTALL=install


OBJS = lua-fiu.c


ifneq ($(V), 1)
	NICE_CC = @echo "  CC  $@"; $(CC)
else
	NICE_CC = $(CC)
endif


default: all
	
all: fiu.so

fiu.so: $(OBJS)
	$(NICE_CC) $(ALL_CFLAGS) -shared -fPIC $(OBJS) \
		-L../../libfiu/ \
		-lfiu -o fiu.so

.c.o:
	$(NICE_CC) $(ALL_CFLAGS) -c $< -o $@

install: fiu.so
	#$(INSTALL) -d $(PREFIX)/lib
	#$(INSTALL) -m 0755 fiu_run_preload.so $(PREFIX)/lib
	#$(INSTALL) -d $(PREFIX)/bin
	#$(INSTALL) -m 0755 fiu-run $(PREFIX)/bin
	#$(INSTALL) -d $(PREFIX)/share/man/man1
	#$(INSTALL) -m 0644 fiu-run.1 $(PREFIX)/share/man/man1/

uninstall:
	$(RM) $(PREFIX)/lib/fiu_run_preload.so
	$(RM) $(PREFIX)/bin/fiu-run
	$(RM) $(PREFIX)/share/man/man1/fiu-run.1

clean:
	rm -f $(OBJS) fiu.so
	rm -f *.bb *.bbg *.da *.gcov *.gcda *.gcno gmon.out

.PHONY: default install uninstall clean .force-build-flags


