#include "telas.h"
#include <allegro5/allegro_primitives.h>

void desenhar_tela_menu(ALLEGRO_FONT* fonte)
{
    al_clear_to_color(al_map_rgb(15, 18, 28));

    al_draw_filled_rectangle(180, 160, 620, 440, al_map_rgb(28, 34, 52));
    al_draw_rectangle(180, 160, 620, 440, al_map_rgb(80, 140, 250), 3.0f);

    al_draw_text(fonte, al_map_rgb(80, 200, 255), 400, 210, ALLEGRO_ALIGN_CENTRE, "FUNCTION RUNNER");
    al_draw_text(fonte, al_map_rgb(200, 200, 210), 400, 240, ALLEGRO_ALIGN_CENTRE, "Graph Parkour - Laboratorio de Funcoes");

    al_draw_text(fonte, al_map_rgb(255, 230, 80), 400, 310, ALLEGRO_ALIGN_CENTRE, "Clique aqui ou pressione [ENTER] para Iniciar");
    al_draw_text(fonte, al_map_rgb(160, 160, 175), 400, 360, ALLEGRO_ALIGN_CENTRE, "Pressione [ESC] para Sair");
}

void desenhar_tela_game_over(ALLEGRO_FONT* fonte)
{
    al_clear_to_color(al_map_rgb(30, 10, 15));

    al_draw_filled_rectangle(200, 190, 600, 410, al_map_rgb(50, 20, 25));
    al_draw_rectangle(200, 190, 600, 410, al_map_rgb(220, 60, 60), 3.0f);

    al_draw_text(fonte, al_map_rgb(255, 80, 80), 400, 235, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
    al_draw_text(fonte, al_map_rgb(230, 210, 210), 400, 275, ALLEGRO_ALIGN_CENTRE, "O robo caiu no abismo do laboratorio!");
    al_draw_text(fonte, al_map_rgb(255, 230, 80), 400, 330, ALLEGRO_ALIGN_CENTRE, "Clique ou [R] para Reiniciar");
}

void desenhar_tela_vitoria(ALLEGRO_FONT* fonte)
{
    al_clear_to_color(al_map_rgb(10, 30, 20));

    al_draw_filled_rectangle(200, 190, 600, 410, al_map_rgb(20, 50, 35));
    al_draw_rectangle(200, 190, 600, 410, al_map_rgb(60, 220, 120), 3.0f);

    al_draw_text(fonte, al_map_rgb(80, 255, 140), 400, 235, ALLEGRO_ALIGN_CENTRE, "SALA CONCLUIDA!");
    al_draw_text(fonte, al_map_rgb(210, 240, 220), 400, 275, ALLEGRO_ALIGN_CENTRE, "Voce projetou a funcao corretamente!");
    al_draw_text(fonte, al_map_rgb(255, 230, 80), 400, 330, ALLEGRO_ALIGN_CENTRE, "Clique ou [R] para Jogar Novamente");
}
