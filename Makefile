CFLAGS := -Wall -Wextra -I./raylib/raylib-5.5_linux_amd64/include

LDFLAGS := -L./raylib/raylib-5.5_linux_amd64/lib
LDLIBS := -l:libraylib.a -lm

OUTPUT := rps

$(OUTPUT): main.c
	$(CC) $(CFLAGS) $(LDFLAGS) main.c $(LDLIBS) -o $(OUTPUT)

WEB_OUTPUT := www/index.html

.PHONY: web
web: $(WEB_OUTPUT)

$(WEB_OUTPUT): main.c
	@mkdir -p www
	emcc -o www/index.html main.c -Os -Wall \
		./raylib/raylib-5.5_webassembly/lib/libraylib.a \
		-I. -Iraylib/raylib-5.5_webassembly/include/ \
		-L. -Lraylib/raylib-5.5_webassembly/lib/ \
		-s USE_GLFW=3 \
		--shell-file raylib/minshell.html \
		-DPLATFORM_WEB \
		--preload-file assets

.PHONY: clean
clean:
	rm -f $(OUTPUT)
	rm -rf www
