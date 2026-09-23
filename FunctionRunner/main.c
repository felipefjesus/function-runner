#include <allegro5/allegro.h>
#include <stdio.h>
#include <stdbool.h>

const int LARGURA_TELA = 800;
const int ALTURA_TELA = 600;
const double FPS = 60.0;

int main(int argc, char** argv) {
    // 1. Inicializa o Allegro
    if (!al_init()) {
        printf("Falha ao inicializar o Allegro!\n");
        return -1;
    }

    // 2. Cria a janela
    ALLEGRO_DISPLAY* janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela) {
        printf("Falha ao criar a janela do Allegro!\n");
        return -1;
    }

    // 3. Cria o temporizador para 60 FPS (dispara a cada 1/60 segundo)
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        printf("Falha ao criar o timer!\n");
        al_destroy_display(janela);
        return -1;
    }

    // 4. Cria a fila de eventos
    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        printf("Falha ao criar a fila de eventos!\n");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return -1;
    }

    // 5. Registra de onde virão os eventos: da janela e do timer
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

    // Inicia o relógio
    al_start_timer(timer);

    bool rodando = true;
    bool redesenhar = true;

    // 6. O Game Loop Principal
    while (rodando) {
        ALLEGRO_EVENT evento;
        // Espera chegar algum evento na fila
        al_wait_for_event(fila_eventos, &evento);

        // Se o evento foi o "tique" do timer (1/60s)
        if (evento.type == ALLEGRO_EVENT_TIMER) {
            redesenhar = true;
        }
        // Se o jogador clicou no 'X' para fechar a janela
        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        }

        // Renderização: só redesenha a tela se o timer disparou e a fila está vazia
        if (redesenhar && al_is_event_queue_empty(fila_eventos)) {
            redesenhar = false;

            // Limpa a tela com preto
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // Atualiza a tela
            al_flip_display();
        }
    }

    // 7. Liberação de memória ao fechar
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);
    al_destroy_display(janela);

    return 0;
}