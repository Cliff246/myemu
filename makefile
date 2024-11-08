CXX      := -gcc
BUILD    := ./build
OBJ_DIR  := $(BUILD)


TARGET  := myemu
INC_DIR := inc
SRC_DIR := src src/runtime

SRC := $(foreach DIR, $(SRC_DIR), $(wildcard $(DIR)/*.c))
OBJECTS  := $(SRC:%.c=$(OBJ_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d) 

CXXFLAGS := -std=c99 -MMD $(addprefix -I,$(INC_DIR))
LDFLAGS  := -L/usr/lib -lm 

all: build $(TARGET)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS)  -c $<  -o $@

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
