# Path Locations
USR_INC   = /usr/include
USR_LIB   = /usr/lib/x86_64-linux-gnu
PREFIX    = /usr/local
MANPREFIX = ${PREFIX}/share/man

# Includes
INCS = -I. \
       -I/usr/include \
       -I${USR_INC}/freetype2 \

# Libraries
LIBS = -lc \
       -lX11 \
       -lfontconfig \
       -lfreetype \
       -lXft \
       -lXtst \
       -lXinerama \
       -lXext \
       -L/usr/lib

# Flags
CFLAGS = -std=gnu99 \
         -O2 \
         -pedantic \
         -pthread \
         -Wall \
         ${INCS} \
         -D_DEFAULT_SOURCE

# Compiler
CC = cc

# Headers
HDR = actions.h \
      bar.h  \
      communications.h \
      completions.h \
      config.h \
      data.h \
      editor.h \
      events.h \
      format.h \
      frame.h \
      globals.h \
      group.h \
      history.h \
      hook.h \
      input.h \
      manage.h \
      messages.h \
      number.h \
      ratpoison.h \
      sbuf.h \
      screen.h \
      split.h \
      window.h \
      xinerama.h

# C sources
SRC = actions.c \
      bar.c \
      communications.c \
      completions.c \
      data.c \
      editor.c \
      events.c \
      format.c \
      frame.c \
      globals.c \
      group.c \
      history.c \
      hook.c \
      input.c \
      ratpoison.c \
      manage.c \
      number.c \
      sbuf.c \
      screen.c \
      split.c \
      window.c \
      xinerama.c

# C objects
OBJS = actions.o \
       bar.o \
       communications.o \
       completions.o \
       data.o \
       editor.o \
       events.o \
       format.o \
       frame.o \
       globals.o \
       group.o \
       history.o \
       hook.o \
       input.o \
       manage.o \
       number.o \
       sbuf.o \
       screen.o \
       split.o \
       window.o \
       xinerama.o

#
# Makefile options
#


# State the "phony" targets
.PHONY: all options clean dist install uninstall


all: options ratpoison

options:
	@echo ratpoison build options:
	@echo "CFLAGS  = ${CFLAGS}"
	@echo "LIBS    = ${LIBS}"
	@echo "CC      = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

ratpoison: ${OBJS} ratpoison.o
	@echo CC -o $@
	@${CC} ${OBJS} ratpoison.o ${LIBS} -o $@ 

clean:
	@echo cleaning
	@rm -f ratpoison *.o ratpoison-${VERSION}.tar.gz

dist: clean
	@echo creating dist tarball
	@mkdir -p ratpoison-${VERSION}
	@cp -R Makefile LICENSE README.md ratpoison.png ratpoison.1 ${SRC} ${HDR} ratpoison-${VERSION}
	@tar -cf ratpoison-${VERSION}.tar ratpoison-${VERSION}
	@gzip ratpoison-${VERSION}.tar
	@rm -rf ratpoison-${VERSION}

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f ratpoison ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/ratpoison
	@echo installing manual page to ${DESTDIR}${MANPREFIX}/man1
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@sed "s/VERSION/${VERSION}/g" < ratpoison.1 > ${DESTDIR}${MANPREFIX}/man1/ratpoison.1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/ratpoison.1

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/ratpoison
	@echo removing manual page from ${DESTDIR}${MANPREFIX}/man1
	@rm -f ${DESTDIR}${MANPREFIX}/man1/ratpoison.1
