CXX      := -gcc
CXXFLAGS := -Wall -std=c99 
LDFLAGS  := -L/usr/lib -lm 
BUILD    := ./build
TARGET   := myemu
OBJ_DIR  := $(BUILD)/objects
INC_DIR	 := src/include src/include/utills src/include/scripting
SRC_DIR  := src src/runtime src/scripting src/utills




SRC_LIST := $(foreach SRC_DIR_ITER,$(SRC_DIR) ,$(SRC_DIR_ITER)/ )

#$(info $(INC_LIST))


INC      := $(foreach INC_DIR_ITER, $(INC_DIR) , $(wildcard $(INC_DIR_ITER)/ ))
SRC      := $(foreach SRC_ITER ,$(SRC_LIST) ,$(wildcard $(SRC_ITER)*.c))    
HEADERS  := $(foreach header, $(INC), $(addprefix -I,$(INC)))

#$(info $(INC))

#$(info $(HEADERS))
all: build $(TARGET)


OBJECTS  := $(SRC:%.c=$(OBJ_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)


$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(HEADERS) -c $< -MMD -o $@

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^ $(LDFLAGS)


-include $(DEPENDENCIES) $(HEADERS)

.PHONY: all build debug release info clean


build:
	@mkdir -p $(OBJ_DIR)

clean:
	@rm $(OBJECTS) $(DEPENDENCIES) 

debug: clean
debug: CXXFLAGS += -DDEBUG -g
debug: all

info:
	@echo "[*] Includes 		${INC}		   "
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"

release: CXXFLAGS += -O2
release: all


