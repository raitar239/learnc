.PHONY: all clean run3

CC = gcc
CFLAGS = -Wall -pthread

SRCS = $(wildcard ./*.c)		# search all .c
TARGETS = $(patsubst ./%.c, ./build/%, $(SRCS))	# files to build

# filtred "on 3" and "on 4"
TARGETS_3 = $(filter ./build/3%, $(TARGETS))

all: ./build $(TARGETS)
./build:
	mkdir -p ./build
./build/%: ./%.c				# $< = source name ; $@ = target name
	$(CC) $(CFLAGS) $< -o $@	

clean:							
	rm -rf ./build
run3:							
	@echo "=== RUNNING TASKS ON 3 ==="
	@for x in $(TARGETS_3); do \
		echo "--- Running $$x ---"; \
		./$$x; \
		echo "Press Enter to continue..."; \
		read _unused; \
		echo ""; \
	done