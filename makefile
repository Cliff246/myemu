CXX      := -gcc
CXXFLAGS := -Wall -std=c99
LDFLAGS  := -L/usr/lib -lm 
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects

TARGET   := myemu
INC_DIR	 := src/include 


INC      := $(wildcard $(INC_DIR)/*.h)
SRC      := $(wildcard src/*.c)         

HEADERS  := $(foreach header, $(INC), $(addprefix -I,$(INC)))
INCLUDE  = -I$(INC)
OBJECTS  := $(SRC:%.c=$(OBJ_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

all: build $(TARGET)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES) $(INCLUDE)

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
	@echo "[*] Includes 		${INC}		   "
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"