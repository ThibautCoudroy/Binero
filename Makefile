# LCC path may be different
lcc=../../GBDK/gbdk/bin/lcc.exe
# Max allocs can be changed depending on space optimizations needed
opt=-Wa-l -Wf--max-allocs-per-node10000 -Wl-m -Wl-j -c -o

src_files_c := $(shell find src -name '*.c')

obj_files_c := $(patsubst src/%.c,bin/%.o,$(src_files_c))
obj_files := $(obj_files_c)

all: $(obj_files)
	$(lcc) -Wa-l -Wl-m -Wl-j -Wm-yC -Wm-yn"BINERO" -o export/binero2.gb $^

bin/%.o: src/%.c $(dir $@)
	mkdir -p $(dir $@)
	$(lcc) $(opt) $@ $^

bin/%.o: src/%.s $(dir $@)
	mkdir -p $(dir $@)
	$(lcc) $(opt) $@ $^
clean:
	rm -f bin
	mkdir bin
