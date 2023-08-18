#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#include "raylib.h"

enum role {
	ROCK,
	PAPER,
	SCISSORS,
};

bool win(enum role r1, enum role r2) {
	switch (r1) {
	case ROCK:
		return (r2 == SCISSORS);
	case PAPER:
		return (r2 == ROCK);
	case SCISSORS:
		return (r2 == PAPER);
	}
}

struct point {
	enum role role;
	Vector2 p;
};

int distanceSquared(Vector2 p1, Vector2 p2) {
	int dx = p1.x - p2.x;
	int dy = p1.y - p2.y;
	return dx * dx + dy * dy;
}

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640

#define N_POINTS 100

#define FONT_SIZE 20
#define RADIUS 16
#define DUEL_DISTANCE (4 * RADIUS * RADIUS)

static struct point *points[N_POINTS];

void populatePoints() {
	for (int i = 0; i < N_POINTS; ++i) {
		struct point *p = malloc(sizeof(struct point));
		p->role = random() % 3;
		p->p = (Vector2){
			random() % SCREEN_WIDTH,
			random() % SCREEN_HEIGHT
		};

		points[i] = p;
	}
}

int weightedRandom(float p0, float p1) {
	float r = (float)random() / (float)RAND_MAX;
	if (r < p0) {
		return 0;
	} else if (r < p0 + p1) {
		return 1;
	} else {
		return -1;
	}
}

void move(struct point *pt) {
	struct point *nearest = NULL;
	int dNearest = INT_MAX;

	// find the nearest point that it can defeat
	for (int i = 0; i < N_POINTS; ++i) {
		if (!win(pt->role, points[i]->role)) {
			continue;
		}
		int d = distanceSquared(pt->p, points[i]->p);
		if (d < dNearest) {
			nearest = points[i];
			dNearest = d;
		}
	}

	int dx, dy;
	if (nearest == NULL) {
		dx = weightedRandom(1.0 / 3.0, 1.0 / 3.0);
		dy = weightedRandom(1.0 / 3.0, 1.0 / 3.0);
	} else {
		float dp;

		dx = nearest->p.x - pt->p.x;
		dp = (float)dx / ((float)abs(dx) + 100.0) * 2.0 / 3.0;
		dx = weightedRandom(1.0 / 3.0, 1.0 / 3.0 + dp);

		dy = nearest->p.y - pt->p.y;
		dp = (float)dy / ((float)abs(dy) + 100.0) * 2.0 / 3.0;
		dy = weightedRandom(1.0 / 3.0, 1.0 / 3.0 + dp);
	}

	pt->p.x += dx;
	pt->p.y += dy;
}

void duel() {
	for (int i = 0; i < N_POINTS; ++i) {
		for (int j = i + 1; j < N_POINTS; ++j) {
			struct point *p1 = points[i];
			struct point *p2 = points[j];

			if (distanceSquared(p1->p, p2->p) > DUEL_DISTANCE) {
				continue;
			}

			if (win(p1->role, p2->role)) {
				p2->role = p1->role;
			}
			if (win(p2->role, p1->role)) {
				p1->role = p2->role;
			}
		}
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

	DrawCircleV(pt->p, RADIUS, color);
	Vector2 v = MeasureTextEx(GetFontDefault(), s, FONT_SIZE, 1);
	DrawText(s, pt->p.x - v.x / 2.0, pt->p.y - v.y / 2.0, FONT_SIZE, BLACK);
}

int main(void) {
	srand(time(NULL));

	populatePoints();

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "rps");

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		BeginDrawing();

			ClearBackground(RAYWHITE);

			for (int i = 0; i < N_POINTS; ++i) {
				move(points[i]);
			}

			duel();

			for (int i = 0; i < N_POINTS; ++i) {
				drawPoint(points[i]);
			}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
