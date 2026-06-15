# Nome do compilador
CXX = g++

# Flags de compilação (C++17 é recomendado por causa do std::variant e smart pointers)
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# Diretórios
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Arquivos fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Nome do executável final
TARGET = purplekai

# Regra principal
all: $(TARGET)

# Linkagem do executável
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilação dos arquivos .cpp para .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para limpar os arquivos gerados
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
