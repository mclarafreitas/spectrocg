#include <GLUT/glut.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <fftw3.h>


#include <cmath>
#include <iostream>
#include <vector>

constexpr int SAMPLE_RATE = 44100;
constexpr int CHANNELS = 2;
constexpr int FFT_SIZE = 512;
constexpr int BANDS = 64;

double* fftIn;
fftw_complex* fftOut;
fftw_plan fftPlan;

std::vector<float> window(FFT_SIZE);
std::vector<float> spectrum(BANDS);

void initWindow() {
    for (int i = 0; i < FFT_SIZE; ++i)
        window[i] = 0.5f * (1.0f - cos(2.0 * M_PI * i / (FFT_SIZE - 1)));
}

void audioCallback(void* udata, Uint8* stream, int len) {
    if (!stream || len < FFT_SIZE * CHANNELS * (int)sizeof(Sint16)) return;

    Sint16* samples = reinterpret_cast<Sint16*>(stream);
    for (int i = 0; i < FFT_SIZE; ++i) {
        float l = samples[2 * i] / 32768.0f;
        float r = samples[2 * i + 1] / 32768.0f;
        float mono = (l + r) * 0.5f;
        fftIn[i] = mono * window[i];
    }

    fftw_execute(fftPlan);

    std::vector<float> magnitudes(FFT_SIZE / 2);
    for (int i = 0; i < FFT_SIZE / 2; ++i) {
        float re = fftOut[i][0];
        float im = fftOut[i][1];
        float mag = sqrtf(re * re + im * im);
        magnitudes[i] = mag;
    }

    int step = (FFT_SIZE / 2) / BANDS;
    for (int i = 0; i < BANDS; ++i) {
        float sum = 0.0f;
        for (int j = i * step; j < (i + 1) * step; ++j)
            sum += magnitudes[j];
        spectrum[i] = std::min(sum / step * 0.01f, 1.0f);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    float radius = 0.3f;

    for (int i = 0; i < BANDS; ++i) {
        float angle = 2 * M_PI * i / BANDS;
        float length = radius + spectrum[i] * 0.5f;

        float x1 = radius * cos(angle);
        float y1 = radius * sin(angle);
        float x2 = length * cos(angle);
        float y2 = length * sin(angle);

        float t = (float)i / BANDS;
        glColor3f(t, 0.2f, 1.0f - t); // gradiente azul→vermelho

        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void idleFunc() {
    glutPostRedisplay(); // Força redesenho contínuo
}

void initGL() {
    glClearColor(0, 0, 0, 1);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo.mp3>" << std::endl;
        return 1;
    }

    // GLUT (OpenGL)
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Espectro Radial");
    initGL();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idleFunc);  // Atualização contínua!

    // SDL2 + Mixer
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Erro SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (Mix_OpenAudio(SAMPLE_RATE, MIX_DEFAULT_FORMAT, CHANNELS, 4096) < 0) {
        std::cerr << "Erro Mix_OpenAudio: " << Mix_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    fftIn = (double*)fftw_malloc(sizeof(double) * FFT_SIZE);
    fftOut = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * FFT_SIZE);
    fftPlan = fftw_plan_dft_r2c_1d(FFT_SIZE, fftIn, fftOut, FFTW_ESTIMATE);
    initWindow();
    Mix_SetPostMix(audioCallback, nullptr);

    Mix_Music* music = Mix_LoadMUS(argv[1]);
    if (!music) {
        std::cerr << "Erro Mix_LoadMUS: " << Mix_GetError() << std::endl;
        return 1;
    }

    Mix_PlayMusic(music, -1);
    glutMainLoop();

    // Libera recursos
    fftw_destroy_plan(fftPlan);
    fftw_free(fftIn);
    fftw_free(fftOut);
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}
