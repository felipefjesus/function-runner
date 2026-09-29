#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h> // ADICIONADO: Necessário para desenhar formas geométricas (o chão e o jogador)
#include <stdio.h>
#include <stdbool.h>

const int LARGURA_TELA = 800;
const int ALTURA_TELA = 600;
const double FPS = 60.0;

// ADICIONADO: Constantes para os parâmetros de física, dimensões e chão
const float GRAVIDADE = 0.5f;
const float PULO = -10.0f;
const float VELOCIDADE_LATERAIS = 4.0f;
const float TAMANHO_PLAYER = 40.0f;
const float PISO_Y = 500.0f;

// ADICIONADO: 1. Definição da struct Player com x, y, vx, vy e estado de contacto com o chão
typedef struct {
    float x;
    float y;
    float vx;
    float vy;
    bool no_chao;
} Player;

int main(int argc, char** argv) {
    if (!al_init()) {
        printf("Falha ao inicializar o Allegro!\n");
        return -1;
    }

    // ADICIONADO: Inicialização do módulo de teclado
    if (!al_install_keyboard()) {
        printf("Falha ao inicializar o teclado!\n");
        return -1;
    }

    // ADICIONADO: Inicialização do addon de primitivas gráficas
    if (!al_init_primitives_addon()) {
        printf("Falha ao inicializar o addon de primitivas!\n");
        return -1;
    }

    ALLEGRO_DISPLAY* janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela) {
        printf("Falha ao criar a janela do Allegro!\n");
        return -1;
    }

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        printf("Falha ao criar o timer!\n");
        al_destroy_display(janela);
        return -1;
    }

    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        printf("Falha ao criar a fila de eventos!\n");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return -1;
    }

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    // ADICIONADO: Registo do teclado como fonte de eventos
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());

    al_start_timer(timer);

    bool rodando = true;
    bool redesenhar = true;

    // ADICIONADO: Inicialização da instância do jogador
    Player player = {
        .x = LARGURA_TELA / 2.0f - TAMANHO_PLAYER / 2.0f,
        .y = 100.0f, // Posicionado no ar para demonstrar a queda inicial
        .vx = 0,
        .vy = 0,
        .no_chao = false
    };

    // ADICIONADO: Controlo de estado das teclas A e D
    bool tecla_A = false;
    bool tecla_D = false;

    while (rodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        // ADICIONADO: 2. Captura dos eventos de tecla pressionada (A, D, Espaço)
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (evento.keyboard.keycode) {
            case ALLEGRO_KEY_A:
                tecla_A = true;
                break;
            case ALLEGRO_KEY_D:
                tecla_D = true;
                break;
            case ALLEGRO_KEY_SPACE:
                // Só permite saltar se estiver apoiado no chão
                if (player.no_chao) {
                    player.vy = PULO;
                    player.no_chao = false;
                }
                break;
            }
        }
        // ADICIONADO: Captura dos eventos de tecla libertada
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
        else if (evento.type == ALLEGRO_EVENT_TIMER) {
            // ADICIONADO: Atualização do vetor de velocidade horizontal consoante as teclas
            player.vx = 0;
            if (tecla_A) player.vx -= VELOCIDADE_LATERAIS;
            if (tecla_D) player.vx += VELOCIDADE_LATERAIS;

            // ADICIONADO: 3. Aplicação da gravidade contínua no vetor vy quando no ar
            if (!player.no_chao) {
                player.vy += GRAVIDADE;
            }

            // ADICIONADO: Atualização das posições x e y
            player.x += player.vx;
            player.y += player.vy;

            // ADICIONADO: 4. Colisão com o piso fixo da sala para impedir a queda fora da tela
            if (player.y + TAMANHO_PLAYER >= PISO_Y) {
                player.y = PISO_Y - TAMANHO_PLAYER; // Ajusta a posição ao nível do chão
                player.vy = 0;                     // Anula a velocidade de queda
                player.no_chao = true;             // Confirma o contacto com o solo
            }

            redesenhar = true;
        }
        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        }

        if (redesenhar && al_is_event_queue_empty(fila_eventos)) {
            redesenhar = false;

            al_clear_to_color(al_map_rgb(0, 0, 0));

            // ADICIONADO: Desenho do piso fixo na tela
            al_draw_filled_rectangle(0, PISO_Y, LARGURA_TELA, ALTURA_TELA, al_map_rgb(250, 250, 250));

            // ADICIONADO: Desenho do retângulo do jogador
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

    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);
    al_destroy_display(janela);

    return 0;
}