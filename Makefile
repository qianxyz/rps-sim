CFLAGS := -Wall -Wextra -I./raylib/raylib-5.5_linux_amd64/include

LDFLAGS := -L./raylib/raylib-5.5_linux_amd64/lib
LDLIBS := -l:libraylib.a -lm

OUTPUT := rps

$(OUTPUT): main.c
	$(CC) $(CFLAGS) $(LDFLAGS) main.c $(LDLIBS) -o $(OUTPUT)

.PHONY: clean
clean:
	rm -f $(OUTPUT)
