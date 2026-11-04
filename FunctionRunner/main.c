#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>
#include "configuracao.h"
#include "jogo.h"

static void ajustar_pasta_assets(void) {
    if (al_filename_exists("assets")) return;
    ALLEGRO_PATH* caminho = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    if (caminho) {
        al_change_directory(al_path_cstr(caminho, ALLEGRO_NATIVE_PATH_SEP));
        al_destroy_path(caminho);
    }
}

int main(void) {
    if (!al_init()) return 1;
    if (!al_install_keyboard()) return 1;
    if (!al_install_mouse()) return 1;
    if (!al_init_primitives_addon()) return 1;
    if (!al_init_image_addon()) return 1;
    al_init_font_addon();
    if (!al_init_ttf_addon()) return 1;

    ajustar_pasta_assets();

    ALLEGRO_DISPLAY* janela = al_create_display(LARGURA, ALTURA);
    if (!janela) return 1;
    al_set_window_title(janela, "Function Runner - Graph Parkour");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        al_destroy_display(janela);
        return 1;
    }

    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return 1;
    }

    Recursos recursos;
    carregar_recursos(&recursos);

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_mouse_event_source());

    Jogo jogo;
    inicializar_jogo(&jogo);

    bool redesenhar = true;
    double tempo_anterior = al_get_time();

    al_start_timer(timer);

    while (jogo.rodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            processar_tecla_down(&jogo, evento.keyboard.keycode);
        } else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            processar_tecla_up(&jogo, evento.keyboard.keycode);
        } else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES) {
            jogo.mouse_x = evento.mouse.x;
            jogo.mouse_y = evento.mouse.y;
        } else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && evento.mouse.button == 1) {
            processar_mouse_down(&jogo, evento.mouse.x, evento.mouse.y);
        } else if (evento.type == ALLEGRO_EVENT_TIMER) {
            double tempo_atual = al_get_time();
            double delta_time = tempo_atual - tempo_anterior;
            tempo_anterior = tempo_atual;

            atualizar_jogo(&jogo, (float)delta_time);
            redesenhar = true;
        } else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            jogo.rodando = false;
        }

        if (redesenhar && al_is_event_queue_empty(fila_eventos)) {
            redesenhar = false;
            desenhar_jogo(&jogo, &recursos);
        }
    }

    liberar_recursos(&recursos);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);
    al_destroy_display(janela);

    return 0;
}
