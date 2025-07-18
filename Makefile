# Nome do executável
TARGET = spectro_radial

# Fonte
SRC = spectro_radial.cpp

# Compilador
CXX = g++

# Flags do compilador
CXXFLAGS = -std=c++11 -I/opt/homebrew/include

# Bibliotecas
LDFLAGS = -L/opt/homebrew/lib \
          -lSDL2 -lSDL2_mixer -lfftw3 \
          -framework OpenGL -framework GLUT -lm

# Regra padrão
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

# Limpar arquivos gerados
clean:
	rm -f $(TARGET)
