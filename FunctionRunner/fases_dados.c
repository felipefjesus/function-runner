#include "fase.h"

// Definicao estrategica das 4 fases do jogo:
// Fase 1: Vao horizontal com plataformas na mesma altura -> f(x) = c
// Fase 2: Patamar direito elevado -> rampa linear f(x) = a*x + b
// Fase 3: Bloco eletrico no teto -> parabola que passa por baixo
// Fase 4: Bloco no teto e espinhos no chao -> onda senoidal que desce e sobe
const Fase FASES[NUM_FASES] = {
    {
        "Fase 1 - Função Constante", "f(x) = c",
        "Ligue as duas plataformas com uma reta horizontal na altura certa.",
        FUNCAO_CONSTANTE, 540.0f, 4.0f, 4.0f,
        1, { "c" }, { 0.0f }, { 0.5f },
        0, { { OBST_TETO, 0, 0, 0, 0 } },
        60.0f
    },
    {
        "Fase 2 - Função Afim", "f(x) = a·x + b",
        "O patamar de saída é mais alto: projete uma rampa de subida.",
        FUNCAO_AFIM, 540.0f, 1.0f, 5.0f,
        2, { "a", "b" }, { 0.0f, 0.0f }, { 0.25f, 0.5f },
        0, { { OBST_TETO, 0, 0, 0, 0 } },
        75.0f
    },
    {
        "Fase 3 - Função Quadrática", "f(x) = a·x² + b·x + c",
        "Passe por baixo do bloco elétrico sem cair no vale de ácido.",
        FUNCAO_QUADRATICA, 540.0f, 4.0f, 4.0f,
        3, { "a", "b", "c" }, { 0.0f, 0.0f, 0.0f }, { 0.05f, 0.1f, 0.5f },
        1, { { OBST_TETO, 3.0f, 5.0f, 3.25f, 12.0f } },
        90.0f
    },
    {
        "Fase 4 - Função Senoidal", "f(x) = A·sin(B·x)",
        "Desça sob o bloco elétrico e suba sobre os espinhos com uma onda.",
        FUNCAO_SENOIDAL, 420.0f, 0.0f, 0.0f,
        2, { "A", "B" }, { 0.0f, 0.0f }, { 0.5f, 0.19634954f },
        2, {
            { OBST_TETO, 1.5f, 2.5f, 0.25f, 9.0f },
            { OBST_ESPINHOS, 5.5f, 6.5f, -4.0f, 1.0f }
        },
        90.0f
    }
};
