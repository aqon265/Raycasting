#include <raylib.h>

#include <math.h>

#define FPS 60

#define windowWidth 1300
#define windowHeight 650
#define windowTitle "Raycasting"
#define windowColor SKYBLUE

#define mapWidth 20
#define mapHeight 20

#define _ 0

int map[mapWidth][mapHeight] =
{
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1},
	{1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1},
	{1, _, _, _, _, _, _, _, _, _, _, 1, 1, 1, 1, 1, 1, _, _, 1},
	{1, _, _, 1, 1, 1, 1, 1, 1, _, _, _, _, _, _, _, 1, _, _, 1},
	{1, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, 1},
	{1, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1},
	{1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1},
	{1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1},
	{1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1},
	{1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1},
	{1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1},
	{1, _, _, 1, _, 1, _, 1, _, _, _, 1, _, _, _, _, 1, _, _, 1},
	{1, _, _, _, _, _, _, _, _, _, _, 1, _, _, _, _, 1, _, _, 1},
	{1, _, _, 1, _, 1, _, 1, _, _, _, 1, _, _, _, _, 1, _, _, 1},
	{1, _, _, _, _, _, _, _, _, _, _, 1, _, _, _, _, 1, _, _, 1},
	{1, _, _, 1, _, 1, _, 1, _, _, _, 1, 1, 1, 1, 1, 1, _, _, 1},
	{1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1},
	{1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

#define floorX 0
#define floorY windowHeight / 2

#define floorWidth windowWidth
#define floorHeight windowHeight / 2

#define floorColor GREEN

int main() {

	SetTargetFPS(FPS);

	InitWindow(windowWidth, windowHeight, windowTitle);

	float playerX = 10;
	float playerY = 10;

	float directionX = 1;
	float directionY = 0;

	float planeX = 0;
	float planeY = 0.66;

	float time = 0;
	float oldTime = 0;

	while (!WindowShouldClose()) {

		BeginDrawing();

		DrawRectangle(floorX, floorY, floorWidth, floorHeight, floorColor);

		for (int i = 0; i < windowWidth; i++) {

			float cameraX = 2 * i / (float)windowWidth - 1;

			float rayDirectionX = directionX + planeX * cameraX;
			float rayDirectionY = directionY + planeY * cameraX;

			int mapX = (int)playerX;
			int mapY = (int)playerY;

			float sideDistanceX;
			float sideDistanceY;

			float deltaDistanceX = (rayDirectionX == 0) ? 1e30 : fabs(1 / rayDirectionX);
			float deltaDistanceY = (rayDirectionY == 0) ? 1e30 : fabs(1 / rayDirectionY);

			float perpendicularWallDistance;

			int stepX;
			int stepY;

			int hit = 0;
			int side;

            if (rayDirectionX < 0) {

                stepX = -1;
                sideDistanceX = (playerX - mapX) * deltaDistanceX;

            }
            else {

                stepX = 1;
                sideDistanceX = (mapX + 1.0 - playerX) * deltaDistanceX;

            }

            if (rayDirectionY < 0) {

                stepY = -1;
                sideDistanceY = (playerY - mapY) * deltaDistanceY;

            }
            else {

                stepY = 1;
                sideDistanceY = (mapY + 1.0 - playerY) * deltaDistanceY;

            }

            while (hit == 0) {

                if (sideDistanceX < sideDistanceY) {

                    sideDistanceX += deltaDistanceX;

                    mapX += stepX;

                    side = 0;

                }
                else {

                    sideDistanceY += deltaDistanceY;

                    mapY += stepY;

                    side = 1;

                }

                if (map[mapX][mapY] > 0) {

                    hit = 1;

                }

            }

            if (side == 0) {

                perpendicularWallDistance = (sideDistanceX - deltaDistanceX);

            }
            else {

                perpendicularWallDistance = (sideDistanceY - deltaDistanceY);

            }

            int wallHeight = (int)(windowHeight / perpendicularWallDistance);

            int drawStart = -wallHeight / 2 + windowHeight / 2;

            if (drawStart < 0) {

            	drawStart = 0;

            }

            int drawEnd = wallHeight / 2 + windowHeight / 2;

            if (drawEnd >= windowHeight) {

            	drawEnd = windowHeight - 1;

            }

            Color color;

            switch (map[mapX][mapY]) {

            	case 1:

            		color = GRAY;

            		break;

            }

            DrawLine((float)i, (float)drawStart, (float)i, (float)drawEnd, color);

		}

		oldTime = time;
		time = GetTime();

		float frameTime = (time - oldTime);

		float movementSpeed = frameTime * 5;
		float rotationSpeed = frameTime * 3;

		if (IsKeyDown(KEY_UP)) {

		    if (map[(int)(playerX + directionX * movementSpeed)][(int)playerY] == 0) {

		        playerX += directionX * movementSpeed;

		    }
		    if (map[(int)playerX][(int)(playerY + directionY * movementSpeed)] == 0) {

		        playerY += directionY * movementSpeed;

		    }

		}
		
		if (IsKeyDown(KEY_DOWN)) {

		    if (map[(int)(playerX - directionX * movementSpeed)][(int)playerY] == 0) {

		        playerX -= directionX * movementSpeed;

		    }
		    if (map[(int)playerX][(int)(playerY - directionY * movementSpeed)] == 0) {

		        playerY -= directionY * movementSpeed;

		    }

		}

		if (IsKeyDown(KEY_LEFT)) {

		    if (map[(int)(playerX - planeX * movementSpeed)][(int)playerY] == 0) {

		        playerX -= planeX * movementSpeed;

		    }
		    if (map[(int)playerX][(int)(playerY + directionY * movementSpeed)] == 0) {

		        playerY -= planeY * movementSpeed;

		    }

		}
		
		if (IsKeyDown(KEY_RIGHT)) {

		    if (map[(int)(playerX + planeX * movementSpeed)][(int)playerY] == 0) {

		        playerX += planeX * movementSpeed;

		    }
		    if (map[(int)playerX][(int)(playerY - directionY * movementSpeed)] == 0) {

		        playerY += planeY * movementSpeed;

		    }

		}

		if (IsKeyDown(KEY_A)) {

		    float olddirectionX = directionX;

		    directionX = directionX * cos(-rotationSpeed) - directionY * sin(-rotationSpeed);
		    directionY = olddirectionX * sin(-rotationSpeed) + directionY * cos(-rotationSpeed);

		    float oldPlaneX = planeX;

		    planeX = planeX * cos(-rotationSpeed) - planeY * sin(-rotationSpeed);
		    planeY = oldPlaneX * sin(-rotationSpeed) + planeY * cos(-rotationSpeed);

		}

		if (IsKeyDown(KEY_D)) {

		    float olddirectionX = directionX;

		    directionX = directionX * cos(rotationSpeed) - directionY * sin(rotationSpeed);
		    directionY = olddirectionX * sin(rotationSpeed) + directionY * cos(rotationSpeed);

		    float oldPlaneX = planeX;

		    planeX = planeX * cos(rotationSpeed) - planeY * sin(rotationSpeed);
		    planeY = oldPlaneX * sin(rotationSpeed) + planeY * cos(rotationSpeed);

		}

		EndDrawing();

		ClearBackground(windowColor);

	}

	CloseWindow();

	return 0;

}