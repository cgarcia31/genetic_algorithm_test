CC = clang
CFLAGS = -fPIC -O2 -I./include
DEBUG_FLAGS = -g -DDEBUG
RM = rm -f

SRC_DIR = src
BUILD_DIR = build
DOC_DIR = doc

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
OBJS_DEBUG = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%_debug.o,$(SRCS))

.PHONY: all debug clean docs

all: find_a_word fitness.so

debug: CFLAGS += $(DEBUG_FLAGS)
debug: find_a_word fitness.so

find_a_word: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

fitness.so: $(BUILD_DIR)/fitness.o
	$(CC) $(CFLAGS) -shared -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%_debug.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

docs:
	doxygen Doxyfile

clean:
	$(RM) find_a_word fitness.so $(OBJS) $(OBJS_DEBUG)
	$(RM) -r $(BUILD_DIR) $(DOC_DIR)
	