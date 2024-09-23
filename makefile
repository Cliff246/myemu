CXX      := -gcc
CXXFLAGS := -pedantic-errors -Wall -std=c99
LDFLAGS  := -L/usr/lib -lm
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects

TARGET   := myemu_old
INCLUDE  := -Iinclude/
SRC      := $(wildcard src/*.c)         \

OBJECTS  := $(SRC:%.c=$(OBJ_DIR)/%.o)
DEPENDENCIES \
         := $(OBJECTS:.o=.d)

all: build $(TARGET)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES)

.PHONY: all build debug release info clean

build:
	@mkdir -p $(OBJ_DIR)

clean:
	@rm $(OBJECTS) $(DEPENDENCIES)
debug: clean
debug: CXXFLAGS += -DDEBUG -g
debug: all



release: CXXFLAGS += -O2
release: all



info:
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"
