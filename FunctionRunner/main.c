#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include <stdbool.h>

const int LARGURA = 800;
const int ALTURA = 600;
const float FPS = 60.0f;

const float GRAVIDADE = 0.5f;
const float FORCA_PULO = -10.0f;
const float VELOCIDADE = 4.0f;
const int TAMANHO_JOGADOR = 40;
const int PISO = 500;

typedef struct {
    float x;
    float y;
    float vx;
    float vy;
    bool no_chao;
} Player;

void desenhar_modal(ALLEGRO_FONT* fonte, float a, float b)
{
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);

    al_draw_filled_rectangle(
        180, 180, 620, 420,
        al_map_rgb(35, 35, 45)
    );

    al_draw_text(
        fonte, branco, 400, 210,
        ALLEGRO_ALIGN_CENTRE,
        "Ajuste da funcao"
    );

    al_draw_text(
        fonte, branco, 400, 240,
        ALLEGRO_ALIGN_CENTRE,
        "y = a * x + b"
    );

    al_draw_textf(
        fonte, branco, 400, 280,
        ALLEGRO_ALIGN_CENTRE,
        "a = %.1f", a
    );

    al_draw_textf(
        fonte, branco, 400, 310,
        ALLEGRO_ALIGN_CENTRE,
        "b = %.1f", b
    );

    al_draw_text(
        fonte, branco, 400, 350,
        ALLEGRO_ALIGN_CENTRE,
        "Esquerda/direita: a | Cima/baixo: b"
    );

    al_draw_text(
        fonte, branco, 400, 380,
        ALLEGRO_ALIGN_CENTRE,
        "ESC: fechar"
    );
}

int main(void)
{
    if (!al_init()) {
        printf("Erro ao iniciar o Allegro.\n");
        return 1;
    }

    if (!al_install_keyboard()) {
        printf("Erro ao iniciar o teclado.\n");
        return 1;
    }

    if (!al_init_primitives_addon()) {
        printf("Erro ao iniciar as formas do Allegro.\n");
        return 1;
    }

    al_init_font_addon();

    ALLEGRO_DISPLAY* janela = al_create_display(LARGURA, ALTURA);

    if (janela == NULL) {
        printf("Erro ao criar a janela.\n");
        return 1;
    }

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

    if (timer == NULL) {
        printf("Erro ao criar o timer.\n");
        al_destroy_display(janela);
        return 1;
    }

    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();

    if (fila_eventos == NULL) {
        printf("Erro ao criar a fila de eventos.\n");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return 1;
    }

    ALLEGRO_FONT* fonte = al_create_builtin_font();

    if (fonte == NULL) {
        printf("Erro ao criar a fonte.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return 1;
    }

    al_register_event_source(
        fila_eventos,
        al_get_display_event_source(janela)
    );

    al_register_event_source(
        fila_eventos,
        al_get_timer_event_source(timer)
    );

    al_register_event_source(
        fila_eventos,
        al_get_keyboard_event_source()
    );

    Player jogador;

    jogador.x = (LARGURA - TAMANHO_JOGADOR) / 2.0f;
    jogador.y = 100;
    jogador.vx = 0;
    jogador.vy = 0;
    jogador.no_chao = false;

    bool rodando = true;
    bool redesenhar = true;
    bool tecla_A = false;
    bool tecla_D = false;
    bool modal_aberta = false;

    float a = 1.0f;
    float b = 0.0f;

    double tempo_anterior = al_get_time();

    al_start_timer(timer);

    while (rodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            int tecla = evento.keyboard.keycode;

            if (modal_aberta) {
                if (tecla == ALLEGRO_KEY_LEFT) {
                    a -= 0.1f;
                }
                else if (tecla == ALLEGRO_KEY_RIGHT) {
                    a += 0.1f;
                }
                else if (tecla == ALLEGRO_KEY_UP) {
                    b += 0.1f;
                }
                else if (tecla == ALLEGRO_KEY_DOWN) {
                    b -= 0.1f;
                }
                else if (tecla == ALLEGRO_KEY_ESCAPE) {
                    modal_aberta = false;
                }
            }
            else {
                if (tecla == ALLEGRO_KEY_E) {
                    modal_aberta = true;
                    tecla_A = false;
                    tecla_D = false;
                    jogador.vx = 0;
                }
                else if (tecla == ALLEGRO_KEY_A) {
                    tecla_A = true;
                }
                else if (tecla == ALLEGRO_KEY_D) {
                    tecla_D = true;
                }
                else if (tecla == ALLEGRO_KEY_SPACE) {
                    if (jogador.no_chao) {
                        jogador.vy = FORCA_PULO;
                        jogador.no_chao = false;
                    }
                }
            }
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_A) {
                tecla_A = false;
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_D) {
                tecla_D = false;
            }
        }
        else if (evento.type == ALLEGRO_EVENT_TIMER) {
            double tempo_atual = al_get_time();
            double delta_time = tempo_atual - tempo_anterior;
            tempo_anterior = tempo_atual;

            float dt_fator = (float)(delta_time * FPS);

            if (!modal_aberta) {
                jogador.vx = 0;

                if (tecla_A) {
                    jogador.vx -= VELOCIDADE;
                }

                if (tecla_D) {
                    jogador.vx += VELOCIDADE;
                }

                if (!jogador.no_chao) {
                    jogador.vy += GRAVIDADE * dt_fator;
                }

                jogador.x += jogador.vx * dt_fator;
                jogador.y += jogador.vy * dt_fator;

                if (jogador.y + TAMANHO_JOGADOR >= PISO) {
                    jogador.y = PISO - TAMANHO_JOGADOR;
                    jogador.vy = 0;
                    jogador.no_chao = true;
                }
            }

            redesenhar = true;
        }
        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        }

        if (redesenhar && al_is_event_queue_empty(fila_eventos)) {
            redesenhar = false;

            al_clear_to_color(al_map_rgb(0, 0, 0));

            al_draw_filled_rectangle(
                0, PISO, LARGURA, ALTURA,
                al_map_rgb(255, 255, 255)
            );

            al_draw_filled_rectangle(
                jogador.x,
                jogador.y,
                jogador.x + TAMANHO_JOGADOR,
                jogador.y + TAMANHO_JOGADOR,
                al_map_rgb(255, 0, 0)
            );

            if (modal_aberta) {
                desenhar_modal(fonte, a, b);
            }

            al_flip_display();
        }
    }

    al_destroy_font(fonte);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);
    al_destroy_display(janela);

    return 0;
}