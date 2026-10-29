#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "configuracao.h"
#include "jogo.h"

int main(void)
{
    if (!al_init()) return 1;
    if (!al_install_keyboard()) return 1;
    if (!al_install_mouse()) return 1; // ADICIONADO: Instalar mouse
    if (!al_init_primitives_addon()) return 1;
    al_init_font_addon();

    ALLEGRO_DISPLAY* janela = al_create_display(LARGURA, ALTURA);
    if (!janela) return 1;

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

    ALLEGRO_FONT* fonte = al_create_builtin_font();
    if (!fonte) {
        al_destroy_event_queue(fila_eventos);
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return 1;
    }

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_mouse_event_source()); // ADICIONADO: Registrar fonte de eventos do mouse

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
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            processar_tecla_up(&jogo, evento.keyboard.keycode);
        }
        else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) { // ADICIONADO: Tratamento de clique do mouse
            processar_mouse_down(&jogo, evento.mouse.x, evento.mouse.y);
        }
        else if (evento.type == ALLEGRO_EVENT_TIMER) {
            double tempo_atual = al_get_time();
            double delta_time = tempo_atual - tempo_anterior;
            tempo_anterior = tempo_atual;

            float dt_fator = (float)(delta_time * FPS);
            atualizar_fisica(&jogo, dt_fator);

            redesenhar = true;
        }
        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            jogo.rodando = false;
        }

        if (redesenhar && al_is_event_queue_empty(fila_eventos)) {
            redesenhar = false;
            desenhar_jogo(&jogo, fonte);
        }
    }

    al_destroy_font(fonte);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);
    al_destroy_display(janela);

    return 0;
}
