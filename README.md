# 🎵 Visualizador de Espectro Radial em C++

Este projeto é um **visualizador de espectro de áudio** que representa as frequências de uma música em formato **radial** (circular), utilizando:

- 🎧 SDL2 + SDL2_mixer para reprodução de áudio
- 📊 FFTW3 para análise de frequências (FFT)
- 🌀 OpenGL (via GLUT) para renderização gráfica

---

## ✨ Objetivo

Este projeto foi desenvolvido como parte de um trabalho de computação gráfica. O objetivo é **visualizar graficamente o espectro de um arquivo de áudio** em tempo real, com uma estética diferente da abordagem de barras horizontais tradicional.

Este visualizador exibe as bandas de frequência em **formato circular**, criando um efeito dinâmico semelhante a um radar ou um medidor radial.

---

## 🖥️ Pré-requisitos (macOS)
(Para tutorial de como executar no Windows ou Linux, por favor seguir o tutorial feito pelo professor [Agostinho Brito](https://github.com/agostinhobritojr/sdlspectrum/)
Antes de compilar, instale as bibliotecas necessárias com [Homebrew](https://brew.sh):

```
brew install sdl2 sdl2_mixer fftw
```
## ⚙️ Compilação

Fiz um arquivo **Makefile** para facilitar a execução então para compilar basta escrever ```make``` no terminal. 
Ou
Compile manualmente:
```
g++ spectro_radial.cpp -o spectro_radial \
    -std=c++11 \
    -I/opt/homebrew/include \
    -L/opt/homebrew/lib \
    -lSDL2 -lSDL2_mixer -lfftw3 \
    -framework OpenGL -framework GLUT -lm
```
Depois de compilar, execute com este comando:
```
./spectro_radial musica.mp3
```

OBS: É necessário um arquivo .mp3 na mesma pasta ou então direcione o caminho para o arquivo no comando.

## 📸 Visual

O espectro é desenhado em formato circular, com cada linha representando a intensidade de uma banda de frequência. O comprimento das linhas varia conforme a energia naquela faixa, criando um visual dinâmico e pulsante.

## 🔧 Observações

Este projeto está adaptado especificamente para macOS, incluindo o uso de frameworks nativos (OpenGL, GLUT) via -framework.

## 🧠 Créditos

Inspirado no exemplo fornecido pelo professor Agostinho Brito do curso de Engenharia de Computação da UFRN, com modificações criativas no estilo de visualização, estrutura e atualização gráfica.
https://github.com/agostinhobritojr/sdlspectrum/

