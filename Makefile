XMEMPOOL_LIB = third-party/xmempool/xmempool.o
OCTREE_OBJ = bkr_octree.o
MINDIFF_OBJ = bkr_mindiff.o
MIX_OBJ = bkr_mix.o
COMMON_OBJ = bkr_common.o
TESTER_OBJ = bkr_test.o
PALETTE_OBJ = bkr_palette.o
STLIB = byakuren
OBJECTS = $(OCTREE_OBJ) $(COMMON_OBJ) $(XMEMPOOL_LIB) $(MINDIFF_OBJ) $(MIX_OBJ) $(PALETTE_OBJ)

TESTER = ./test/bkr_test

CC = gcc
OPTIMIZATION ?= -O2
WARNINGS = -Wno-missing-field-initializers
DEBUG ?= -g -ggdb

REAL_CFLAGS = $(OPTIMIZATION) -fPIC $(CFLAGS) $(WARNINGS) $(DEBUG) -std=c99

$(STLIB): $(OBJECTS)
	ar rcs $(STLIB).a $(OBJECTS)

$(XMEMPOOL_LIB): third-party/xmempool/xmempool.c
	cd ./third-party/xmempool/ && make

$(COMMON_OBJ): ./common.c ./common.h
	$(CC) -c $(REAL_CFLAGS) $< -o $@

$(OCTREE_OBJ): ./lib/octree.c ./lib/octree.h ./common.h
	$(CC) -c $(REAL_CFLAGS) $< -o $@

$(MINDIFF_OBJ): ./lib/mindiff.c ./lib/mindiff.h ./common.h
	$(CC) -c $(REAL_CFLAGS) $< -o $@

$(MIX_OBJ): ./lib/mix.c ./lib/mix.h ./common.h ./lib/octree.h ./lib/mindiff.h
	$(CC) -c $(REAL_CFLAGS) $< -o $@

$(PALETTE_OBJ): ./const/palette.c ./const/palette.h ./common.h
	$(CC) -c $(REAL_CFLAGS) $< -o $@

$(TESTER): $(TESTER_OBJ) $(STLIB)
	$(CC) -o $@ $(REAL_CFLAGS) $< -I. $(STLIB).a

$(TESTER_OBJ): ./test/test.c ./byakuren.h
	$(CC) -c $(REAL_CFLAGS) $< -o $@

clean:
	$(RM) -rf *.o *.a test/bkr_test
	cd ./third-party/xmempool/ && make clean

%.compdb_entry: %.c
	@echo "    {" > $@
	@echo "        \"command\": \"$(CC) $(REAL_CFLAGS) -c $<\","   >> $@
	@echo "        \"directory\": \"$(CURDIR)\","               >> $@
	@echo "        \"file\": \"$<\""                    >> $@
	@echo "    },"                              >> $@

COMPDB_ENTRIES = $(addsuffix .compdb_entry, $(basename \
	./common.c \
	./lib/octree.c \
	./lib/mindiff.c \
	./const/palette.c \
	./test/test.c \
	./third-party/xmempool/xmempool.c))

compile_commands.json: $(COMPDB_ENTRIES)
	@echo "[" > $@.tmp
	@cat $^ >> $@.tmp
	@sed '$$d' < $@.tmp > $@
	@echo "    }" >> $@
	@echo "]" >> $@
	@rm $(COMPDB_ENTRIES)
	@rm $@.tmp
