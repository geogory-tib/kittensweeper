#include <raylib.h>




int main(void) {
  InitWindow(500, 500, "500");
  Texture2D test =  LoadTexture("deadyellowcat.png");
  for (; !WindowShouldClose();) {
    BeginDrawing();
	DrawTexture(test,0,0,WHITE);
	EndDrawing();
  }
}
