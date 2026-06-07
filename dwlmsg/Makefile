BINS = dwlmsg

PREFIX ?= /usr/local
CFLAGS += -Wall -Wextra -Wno-unused-parameter
PKG_CONFIG = pkg-config
PKGS = wayland-client
CFLAGS += `$(PKG_CONFIG) --cflags $(PKGS)`
LDLIBS += `$(PKG_CONFIG) --libs $(PKGS)`

all: $(BINS)

clean:
	rm -f $(BINS) *.o *-protocol.*

install: all
	mkdir -p $(PREFIX)/bin
	cp -f $(BINS) $(PREFIX)/bin

WAYLAND_PROTOCOLS = `pkg-config --variable=pkgdatadir wayland-protocols`
WAYLAND_SCANNER = `pkg-config --variable=wayland_scanner wayland-scanner`

dwlmsg: dwlmsg.o dwl-ipc-unstable-v2-protocol.o
	$(CC) dwlmsg.o dwl-ipc-unstable-v2-protocol.o $(CFLAGS) $(LDLIBS) -o $@

dwlmsg.o: dwlmsg.c dwl-ipc-unstable-v2-protocol.h

dwl-ipc-unstable-v2-protocol.h:
	$(WAYLAND_SCANNER) client-header \
		protocols/dwl-ipc-unstable-v2.xml $@
dwl-ipc-unstable-v2-protocol.c:
	$(WAYLAND_SCANNER) private-code \
		protocols/dwl-ipc-unstable-v2.xml $@
