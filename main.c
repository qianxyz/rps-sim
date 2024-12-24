#include <float.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"

enum role {
	ROCK,
	PAPER,
	SCISSORS,
};

/* win[i][j] = 1 if i wins j */
int win[3][3] = {
	{ 0, 0, 1 },
	{ 1, 0, 0 },
	{ 0, 1, 0 },
};

struct point {
	enum role role;
	Vector2 p;
};

float distanceSquared(Vector2 p1, Vector2 p2)
{
	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;
	return dx * dx + dy * dy;
}

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640

#define N_POINTS 80

/* each point is modeled as a disk */
#define RADIUS 20.0

static struct point *points[N_POINTS];

void initWorld()
{
	for (int i = 0; i < N_POINTS; ++i) {
		struct point *p = malloc(sizeof(struct point));
		p->role = random() % 3;
		p->p = (Vector2){ random() % SCREEN_WIDTH,
				  random() % SCREEN_HEIGHT };

		points[i] = p;
	}
}

int weightedRandom(float p0, float p1)
{
	float r = (float)random() / (float)RAND_MAX;
	if (r < p0) {
		return 0;
	} else if (r < p0 + p1) {
		return 1;
	} else {
		return -1;
	}
}

void move(struct point *pt)
{
	struct point *nearest = NULL;
	float dNearest = FLT_MAX;

	// find the nearest point that it can defeat
	for (int i = 0; i < N_POINTS; ++i) {
		if (!win[pt->role][points[i]->role]) {
			continue;
		}
		float d = distanceSquared(pt->p, points[i]->p);
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

void duel()
{
	struct point *p1, *p2;

	for (int i = 0; i < N_POINTS; ++i) {
		for (int j = i + 1; j < N_POINTS; ++j) {
			p1 = points[i];
			p2 = points[j];

			if (distanceSquared(p1->p, p2->p) >
			    4 * RADIUS * RADIUS) {
				continue;
			}

			if (win[p1->role][p2->role]) {
				p2->role = p1->role;
			}
			if (win[p2->role][p1->role]) {
				p1->role = p2->role;
			}
		}
	}
}

Texture2D textures[3];

void loadTextures()
{
	textures[ROCK] = LoadTexture("assets/rock.png");
	textures[PAPER] = LoadTexture("assets/paper.png");
	textures[SCISSORS] = LoadTexture("assets/scissors.png");
}

void drawPoint(struct point *pt)
{
	Texture2D *t = &textures[pt->role];

	Rectangle source = { 0.0f, 0.0f, (float)t->width, (float)t->height };
	Rectangle dest = { pt->p.x - RADIUS, pt->p.y - RADIUS, RADIUS * 2.0,
			   RADIUS * 2.0 };
	Vector2 origin = { 0.0f, 0.0f };
	DrawTexturePro(*t, source, dest, origin, 0.0f, WHITE);
}

int main(void)
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "rps");
	// textures must be loaded after `InitWindow`
	loadTextures();

	SetTargetFPS(30);

	srand(time(NULL));
	initWorld();

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
