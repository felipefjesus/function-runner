#include <allegro5/allegro.h>
#include <stdio.h>

int main() {
		
	if (!al_init()) {
		printf("Falha ao iniciar o Allegro!\n");
		return -1;
	}

	ALLEGRO_DISPLAY *janela = al_create_display(800, 600);
	if (!janela) {
		printf("Falha ao criar a janela do Aleefro!\n");
		return -1;
	}

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();

	al_rest(3.0);

	al_destroy_display(janela);

	return 0;
}