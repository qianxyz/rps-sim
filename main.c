#include <time.h>
#include <stdlib.h>

#include "raylib.h"

enum role {
	ROCK,
	PAPER,
	SCISSORS,
};

struct point {
	enum role role;
	Vector2 p;
};

#define SCREENWIDTH 640
#define SCREENHEIGHT 640
#define NPOINTS 100

static struct point *points[NPOINTS];

void populatePoints() {
	for (int i = 0; i < NPOINTS; ++i) {
		struct point *p = malloc(sizeof(struct point));
		p->role = random() % 3;
		p->p = (Vector2){ random() % SCREENWIDTH, random() % SCREENHEIGHT };

		points[i] = p;
	}
}

void drawPoint(struct point *pt) {
	char *s;
	Color color;

	switch (pt->role) {
	case ROCK:
		s = "r";
		color = RED;
		break;
	case PAPER:
		s = "p";
		color = GREEN;
		break;
	case SCISSORS:
		s = "s";
		color = BLUE;
		break;
	}

	DrawText(s, pt->p.x, pt->p.y, 30, color);
}

int main(void) {
	srand(time(NULL));

	populatePoints();

	InitWindow(SCREENWIDTH, SCREENHEIGHT, "rps");

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		BeginDrawing();

			ClearBackground(RAYWHITE);

			for (int i = 0; i < NPOINTS; ++i) {
				points[i]->p.x += random() % 3 - 1;
				points[i]->p.y += random() % 3 - 1;
			}

			for (int i = 0; i < NPOINTS; ++i) {
				drawPoint(points[i]);
			}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
