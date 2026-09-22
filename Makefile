PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin
TARGET = qui

all: $(TARGET)

$(TARGET): qui.c
	$(CC) $(CFLAGS) -o $(TARGET) qui.c

# 설치 타깃
install: $(TARGET)
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)

# 삭제 타깃
uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(TARGET)

.PHONY: all install uninstall