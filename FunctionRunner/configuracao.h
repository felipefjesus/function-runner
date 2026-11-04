#ifndef CONFIGURACAO_H
#define CONFIGURACAO_H

// Dimensoes da janela e taxa de atualizacao
#define LARGURA 800
#define ALTURA 600
#define FPS 60.0f

// Quantidade de fases e limites das tabelas
#define NUM_FASES 4
#define MAX_COEF 3
#define MAX_OBSTACULOS 3

// Constantes de fisica do robo
#define GRAVIDADE 0.5f
#define FORCA_PULO -10.0f
#define VELOCIDADE 4.0f
#define TAMANHO_JOGADOR 40
#define INCLINACAO_MAXIMA 1.8f
#define DEGRAU_MAXIMO 6.0f

// Plano cartesiano do vao central (x vai de 0 a 8 unidades; 1 unidade = 40 pixels)
#define UNIDADE 40.0f
#define VAO_INICIO 240.0f
#define VAO_FIM 560.0f
#define NIVEL_ACIDO 560.0f

// Posicoes dos objetos interativos da sala
#define LAPIS_X 205.0f
#define RAIO_INTERACAO_LAPIS 60.0f
#define PORTA_X 700.0f
#define PORTA_LARGURA 48.0f
#define PORTA_ALTURA 80.0f

// Estados da maquina de estados do jogo (Cartao 07)
typedef enum {
    MENU,
    SELECAO_FASE,
    COMO_JOGAR,
    JOGANDO,
    PAUSA,
    VITORIA,
    GAME_OVER,
    FIM_DE_JOGO
} EstadoJogo;

// Motivo da derrota para exibir na tela de Game Over
typedef enum {
    MORTE_ABISMO,
    MORTE_OBSTACULO,
    MORTE_TEMPO
} MotivoDerrota;

#endif
