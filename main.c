#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define W 30
#define H 18
#define MAX_ENEMIES 8

typedef struct {
    int x, y;
    int hp;
    int alive;
} Enemy;

static int px = W / 2;
static int py = H / 2;
static int level = 1;
static int xp = 0;
static int hp = 5;
static int score = 0;
static int attackCooldown = 0;
static int spawnCooldown = 0;
static Enemy enemies[MAX_ENEMIES];

static int abs_i(int v) { return v < 0 ? -v : v; }

static void spawn_enemy(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].alive) {
            int side = rand() % 4;
            if (side == 0) { enemies[i].x = 1; enemies[i].y = 1 + rand() % (H - 2); }
            if (side == 1) { enemies[i].x = W - 2; enemies[i].y = 1 + rand() % (H - 2); }
            if (side == 2) { enemies[i].x = 1 + rand() % (W - 2); enemies[i].y = 1; }
            if (side == 3) { enemies[i].x = 1 + rand() % (W - 2); enemies[i].y = H - 2; }
            enemies[i].hp = 1 + (level > 2);
            enemies[i].alive = 1;
            return;
        }
    }
}

static void draw_game(void) {
    consoleClear();
    printf("        POKEBONK: 2DS MINI RECREATION\n");
    printf("  Move: D-pad   Attack: A   Quit: START\n");
    printf("  Level: %d   XP: %d/%d   HP: %d   Bonks: %d\n",
           level, xp, level * 5, hp, score);
    printf("+");
    for (int x = 0; x < W; x++) printf("-");
    printf("+\n");

    for (int y = 0; y < H; y++) {
        printf("|");
        for (int x = 0; x < W; x++) {
            char c = ' ';
            if (x == px && y == py) c = '@';
            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (enemies[i].alive && enemies[i].x == x && enemies[i].y == y)
                    c = 'o';
            }
            printf("%c", c);
        }
        printf("|\n");
    }
    printf("+");
    for (int x = 0; x < W; x++) printf("-");
    printf("+\n");
    printf("@ = you   o = wild Pokemon (enemy)\n");
    printf("Bonk nearby enemies with A. Survive and level up!\n");
}

int main(void) {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
    srand((unsigned int)osGetTime());

    for (int i = 0; i < MAX_ENEMIES; i++) enemies[i].alive = 0;
    spawn_enemy();
    spawn_enemy();

    int gameOver = 0;
    while (aptMainLoop() && !gameOver) {
        hidScanInput();
        u32 down = hidKeysDown();
        u32 held = hidKeysHeld();

        if (down & KEY_START) break;

        if (down & KEY_UP)    py--;
        if (down & KEY_DOWN)  py++;
        if (down & KEY_LEFT)  px--;
        if (down & KEY_RIGHT) px++;

        if (px < 1) px = 1;
        if (px > W - 2) px = W - 2;
        if (py < 1) py = 1;
        if (py > H - 2) py = H - 2;

        if (attackCooldown > 0) attackCooldown--;
        if ((down & KEY_A) && attackCooldown == 0) {
            attackCooldown = 12;
            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (enemies[i].alive &&
                    abs_i(enemies[i].x - px) <= 2 &&
                    abs_i(enemies[i].y - py) <= 2) {
                    enemies[i].hp--;
                    if (enemies[i].hp <= 0) {
                        enemies[i].alive = 0;
                        xp++;
                        score++;
                        if (xp >= level * 5) {
                            xp = 0;
                            level++;
                            hp++;
                        }
                    }
                }
            }
        }

        /* Enemies slowly move toward the player. */
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (!enemies[i].alive) continue;
            if (rand() % 5 == 0) {
                if (enemies[i].x < px) enemies[i].x++;
                else if (enemies[i].x > px) enemies[i].x--;
                if (enemies[i].y < py) enemies[i].y++;
                else if (enemies[i].y > py) enemies[i].y--;
            }
            if (enemies[i].x == px && enemies[i].y == py) {
                enemies[i].alive = 0;
                hp--;
                if (hp <= 0) gameOver = 1;
            }
        }

        if (spawnCooldown > 0) spawnCooldown--;
        if (spawnCooldown == 0) {
            spawn_enemy();
            spawnCooldown = 45;
        }

        draw_game();
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    consoleClear();
    if (gameOver) {
        printf("        GAME OVER!\n\n");
        printf("      Bonks: %d\n", score);
        printf("      Level: %d\n\n", level);
    } else {
        printf("Thanks for playing!\n");
    }
    printf("Press A to exit.\n");
    while (aptMainLoop()) {
        hidScanInput();
        if (hidKeysDown() & KEY_A) break;
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}
