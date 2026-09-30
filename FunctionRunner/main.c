#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>

const int LARGURA_TELA = 800;
const int ALTURA_TELA = 600;
const double FPS = 60.0;

// Constantes de física, dimensões e piso
const float GRAVIDADE = 0.5f;
const float PULO = -10.0f;
const float VELOCIDADE_LATERAIS = 4.0f;
const float TAMANHO_PLAYER = 40.0f;
const float PISO_Y = 500.0f;

// Definição da struct Player com posições, velocidades e contato com o chão
typedef struct {
    float x;
    float y;
    float vx;
    float vy;
    bool no_chao;
} Player;

int main(int argc, char** argv) {
    // 1. Inicialização base do Allegro
    if (!al_init()) {
        printf("Falha ao inicializar o Allegro!\n");
        return -1;
    }

    // 2. Inicialização do teclado
    if (!al_install_keyboard()) {
        printf("Falha ao inicializar o teclado!\n");
        return -1;
    }

    // 3. Inicialização do addon de primitivas gráficas
    if (!al_init_primitives_addon()) {
        printf("Falha ao inicializar o addon de primitivas!\n");
        return -1;
    }

    // 4. Criação da janela
    ALLEGRO_DISPLAY* janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela) {
        printf("Falha ao criar a janela do Allegro!\n");
        return -1;
    }

    // 5. Configuração do timer cravado a 60 FPS
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        printf("Falha ao criar o timer!\n");
        al_destroy_display(janela);
        return -1;
    }

    // 6. Criação da fila de eventos
    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        printf("Falha ao criar a fila de eventos!\n");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return -1;
    }

    // 7. Registro das fontes de eventos
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());

    al_start_timer(timer);

    bool rodando = true;
    bool redesenhar = true;

    // Inicialização da instância do jogador
    Player player = {
        .x = LARGURA_TELA / 2.0f - TAMANHO_PLAYER / 2.0f,
        .y = 100.0f,
        .vx = 0,
        .vy = 0,
        .no_chao = false
    };

    // Controle de estado das teclas A e D
    bool tecla_A = false;
    bool tecla_D = false;

    // Variáveis para medição do Delta Time
    double tempo_anterior = al_get_time();
    double delta_time = 0.0;

    // Game Loop Principal
    while (rodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        // Captura de teclas pressionadas (A, D, Espaço)
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (evento.keyboard.keycode) {
            case ALLEGRO_KEY_A:
                tecla_A = true;
                break;
            case ALLEGRO_KEY_D:
                tecla_D = true;
                break;
            case ALLEGRO_KEY_SPACE:
                if (player.no_chao) {
                    player.vy = PULO;
                    player.no_chao = false;
                }
                break;
            }
        }
        // Captura de teclas liberadas
        else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            switch (evento.keyboard.keycode) {
            case ALLEGRO_KEY_A:
                tecla_A = false;
                break;
            case ALLEGRO_KEY_D:
                tecla_D = false;
                break;
            }
        }
        // Atualização da lógica e física pelo Timer
        else if (evento.type == ALLEGRO_EVENT_TIMER) {
            // Medição do tempo decorrido entre os quadros
            double tempo_atual = al_get_time();
            delta_time = tempo_atual - tempo_anterior;
            tempo_anterior = tempo_atual;

            // Fator multiplicador do Delta Time (normalizado em 1.0 para 60 FPS estáveis)
            float dt_fator = (float)(delta_time * FPS);

            // Atualização da velocidade horizontal com base nas teclas
            player.vx = 0;
            if (tecla_A) player.vx -= VELOCIDADE_LATERAIS;
            if (tecla_D) player.vx += VELOCIDADE_LATERAIS;

            // Aplicação da gravidade proporcional ao tempo decorrido
            if (!player.no_chao) {
                player.vy += GRAVIDADE * dt_fator;
            }

            // Deslocamento aplicando a multiplicação do delta time
            player.x += player.vx * dt_fator;
            player.y += player.vy * dt_fator;

            // Colisão com o chão fixo
            if (player.y + TAMANHO_PLAYER >= PISO_Y) {
                player.y = PISO_Y - TAMANHO_PLAYER;
                player.vy = 0;
                player.no_chao = true;
            }

            redesenhar = true;
        }
        // Fechamento da janela
        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        }

        // Renderização gráfica
        if (redesenhar && al_is_event_queue_empty(fila_eventos)) {
            redesenhar = false;

            al_clear_to_color(al_map_rgb(0, 0, 0));

            // Desenha o chão fixo
            al_draw_filled_rectangle(0, PISO_Y, LARGURA_TELA, ALTURA_TELA, al_map_rgb(250, 250, 250));

            // Desenha o jogador
            al_draw_filled_rectangle(
                player.x,
                player.y,
                player.x + TAMANHO_PLAYER,
                player.y + TAMANHO_PLAYER,
                al_map_rgb(255, 100, 100)
            );

            al_flip_display();
        }
    }

    // Liberação de recursos ao encerrar
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);
    al_destroy_display(janela);

    return 0;
}