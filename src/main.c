#include "raylib.h"
#include "raymath.h"

// max num of enemies that can exist at once
#define MAX_UFOS 50
// max num of lasers that can exist at once
#define MAX_LASERS 50

#define NULL ((void *)0)

typedef struct {
    Vector2 position;
    float scale;
    const Texture2D *texture;
} Sprite;


void RenderSprite(Sprite sprite) {
    DrawTextureEx(*sprite.texture, sprite.position, 0.0, sprite.scale, WHITE);
}

typedef struct {
    Vector2 position;
    float scale;
    Vector2 speed;
    const Texture2D *texture;
    bool active;
} Ufo;

void SpawnUfo(Ufo ufos[], int max_ufos, const Texture2D textures[], int screen_width, float scale, float speed) {
    for (int i = 0; i < max_ufos; i++) {
        if (!ufos[i].active) {
            int texture_index = GetRandomValue(0, 3);
            const Texture2D *texture = &textures[texture_index];

            float start_x = (float)GetRandomValue(0, screen_width - (texture->width) * scale);
            float start_y = 0.0f;
            ufos[i].position = (Vector2){start_x, start_y};
            ufos[i].scale = scale;
            ufos[i].active = true;
            ufos[i].texture = texture;
            ufos[i].speed = (Vector2){0.0f, speed};
            return;
        }
    } 
}

void UpdateUfos(Ufo ufos[], int max_ufos, float delta_time, int screen_height) {
    for (int i = 0; i < max_ufos; i++) {
        if (!ufos[i].active) continue;

        ufos[i].position.y += ufos[i].speed.y * delta_time;

        if (ufos[i].position.y > screen_height) {
            ufos[i].active = false;
        }
    }
}
void RenderUfo(Ufo ufo) {
    DrawTextureEx(*ufo.texture, ufo.position, 0.0, ufo.scale, WHITE);
}

void RenderUfos(Ufo ufos[], int max_ufos) {
    for (int i = 0; i < max_ufos; i++) {
        if (ufos[i].active) RenderUfo(ufos[i]);
    }
}

typedef struct {
    Vector2 position;
    float scale;
    Vector2 speed;
    const Texture2D *texture;
    bool active;
} Laser;

void SpawnLaser(Laser lasers[], int max_lasers, const Texture2D* texture, float scale, float speed, Vector2 start_pos) {
    for (int i = 0; i < max_lasers; i++) {
        if (!lasers[i].active) {
            lasers[i].position = start_pos;
            lasers[i].scale = scale;
            lasers[i].active = true;
            lasers[i].speed = (Vector2){0.0f, speed};
            lasers[i].texture = texture;
            return;
        }
    }
}

void UpdateLasers(Laser lasers[], int max_lasers, float delta_time) {
    for (int i = 0; i < max_lasers; i++) {
        if (!lasers[i].active) continue;
        
        lasers[i].position.y += lasers[i].speed.y * delta_time;

        if (lasers[i].texture != NULL && lasers[i].position.y < -(lasers[i].texture->height * lasers[i].scale)) {
            lasers[i].active = false;
        }
    }
}

void RenderLaser(Laser laser) {
    DrawTextureEx(*laser.texture, laser.position, 0.0, laser.scale, WHITE);
}

void RenderLasers(Laser lasers[], int max_lasers) {
    for (int i = 0; i < max_lasers; i++) {
        if (lasers[i].active) RenderLaser(lasers[i]);
    }
}

Rectangle GetUfoRec(Ufo ufo) {
    return (Rectangle){
        ufo.position.x,
        ufo.position.y,
        ufo.texture->width * ufo.scale,
        ufo.texture->height * ufo.scale
    };
}

Rectangle GetLaserRec(Laser laser) {
    return (Rectangle){
        laser.position.x,
        laser.position.y,
        laser.texture->width * laser.scale,
        laser.texture->height * laser.scale
    };
}

int main(void) {
    const int screen_width = 800;
    const int screen_height = 600;
    const float PLAYER_SPEED = 400.0f;
    const float UFO_SPEED = 100.0f;

    InitWindow(screen_width, screen_height, "Raylib Shooter");
    SetTargetFPS(60);

    const Texture2D ship_texture = LoadTexture("assets/SpaceShooterRedux/PNG/playerShip1_blue.png");
    
    const Texture2D ufo_textures[4] = {
        LoadTexture("assets/SpaceShooterRedux/PNG/ufoBlue.png"),
        LoadTexture("assets/SpaceShooterRedux/PNG/ufoGreen.png"),
        LoadTexture("assets/SpaceShooterRedux/PNG/ufoRed.png"),
        LoadTexture("assets/SpaceShooterRedux/PNG/ufoYellow.png")
    };

    const Texture2D laser_texture = LoadTexture("assets/SpaceShooterRedux/PNG/Lasers/laserRed01.png");

    Sprite player = {
        {(float)screen_width / 2, ((float)screen_height / 4) * 3}, 
        1.0,
        &ship_texture
    };

    // pool of ufos
    Ufo ufos[MAX_UFOS] = { 0 };

    // pool of lasers
    Laser lasers[MAX_LASERS] = { 0 };

    float timer = 0.0f;
    const float ufo_interval = 1.0f;

    while (!WindowShouldClose()) {

        timer += GetFrameTime();
        
        if (timer >= ufo_interval) {
            TraceLog(LOG_INFO, "1 second has passed!");
            SpawnUfo(ufos, MAX_UFOS, ufo_textures, screen_width, 0.5f, UFO_SPEED);
            timer -= ufo_interval;
        }



        if (IsKeyDown(KEY_LEFT))  player.position.x -= PLAYER_SPEED * GetFrameTime();
        if (IsKeyDown(KEY_RIGHT)) player.position.x += PLAYER_SPEED * GetFrameTime();
        if (IsKeyDown(KEY_UP))    player.position.y -= PLAYER_SPEED * GetFrameTime();
        if (IsKeyDown(KEY_DOWN))  player.position.y += PLAYER_SPEED * GetFrameTime();

        if (IsKeyPressed(KEY_SPACE)) {
            Vector2 laser_start = {player.position.x + player.texture->width / 2, player.position.y};
            SpawnLaser(lasers, MAX_LASERS, &laser_texture, 1.0f, -500.0f, laser_start);
        }

        float player_width = player.texture->width * player.scale;
        float player_height = player.texture->height * player.scale;
        player.position.x = Clamp(player.position.x, 0.0f, screen_width - player_width);
        player.position.y = Clamp(player.position.y, 0.0f, screen_height - player_height);

        UpdateUfos(ufos, MAX_UFOS, GetFrameTime(), screen_height);
        UpdateLasers(lasers, MAX_LASERS, GetFrameTime());
        // Render
        BeginDrawing();
            ClearBackground(DARKGRAY);

            RenderUfos(ufos, MAX_UFOS);
            
            RenderLasers(lasers, MAX_LASERS);

            RenderSprite(player);
            
        EndDrawing();
    }

    UnloadTexture(ship_texture);
    UnloadTexture(laser_texture);
    for (int i = 0; i < 4; i++) {
        UnloadTexture(ufo_textures[i]);
    }
    CloseWindow();
    return 0;
}