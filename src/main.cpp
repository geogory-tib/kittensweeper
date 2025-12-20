#include <cassert>
#include <csignal>
#include <cstdlib>
#include <cstdio>
#include <time.h>
#include "include/raylib.h"
#include "include/gtd.h"
#include "include/flood_hash.h"
#define WINDOW_WIDTH 486

#define WINDOW_HEIGH 586

#define GRID_WIDTH 486

#define GRID_HEIGHT 486

#define GRID_START 100

#define APPEND_QUEUE(queue,x,y) queue.append({x,y});

#define BEGIN_MODE 0

#define BEGIN_MINE 10

#define INTER_MODE 1

#define INTER_MINE 40

#define EXPERT_MODE 2

#define EXPERT_MINE 99

const Color BackGroundColor{
  .r = 192,
  .g = 192,
  .b = 192,
  .a = 255,
};

Vector2 SqaureSize;

struct RestartButton_t {
  Rectangle RestartRect{
      .x = (WINDOW_WIDTH / 2.0f) - (67.5 / 2.0f),
      .y = (100 / 2.0f) - (67.5 / 2.0f),
      .width = 67.5,
      .height = 67.5,
  };
  bool Pressed = false;
  bool Dead = false; // this is the game over con
  bool Won = false;
};

RestartButton_t RestartButton;

Texture2D DefaultBoxTexture;

Texture2D PressedBoxTexture;

Texture2D EvilMineCat;

Texture2D YellowCatAlive;

Texture2D YellowCatDead;

Texture2D YellowCatWon;

Texture2D YellowCatDeadPressed;

Texture2D YellowCatAlivePressed;

Texture2D FlaggedCellTexture;

Texture2D NumberAssests[8];

struct Cell {
  bool IsAMine = false;
  bool Pressed = false;
  bool Flagged = false;
  Vector2 postion;
  int MinesInArea;
};



// can be scaled in the future for now just assume 9x9 grid
int GameDif = BEGIN_MODE;
Cell *MineGrid = NULL;
int GridRows = 9;
int GridCols = 9;
int TotalMines;
int NonMinesLeft;

inline void render_restart_button();

void generate_random_mines(int in,int total_mines,Vector2I *mine_array);

void flood_fill(int Sx, int Sy);

bool flood_check(Vector2I *queue, int ql, Vector2I cord);

void init_mine_grid();

inline void handle_cell_texture(Cell current_cell,Rectangle TextureSource,Rectangle concated_shape);

void load_assests();

void unload_assests();

void render_game_state();

void calculate_mines_in_area(Vector2I *mine_array);

void handle_resize();

void update_mine_counter();

Cell *get_cell(int x,int y);

void get_mine_size();

int main(void) {
  init_mine_grid();
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGH, "KittenSweeper! :3");
  SetTargetFPS(60);
  load_assests();
  BeginDrawing();
  ClearBackground(BackGroundColor);
  render_game_state();
  EndDrawing();
  for (; !WindowShouldClose() && !IsKeyPressed(KEY_Q);) {
	BeginDrawing();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
	  Vector2 mouse_pos = GetMousePosition();
	  if ((!RestartButton.Dead && !RestartButton.Won) && mouse_pos.y >= 100){
		 int gridX = int((mouse_pos.x / SqaureSize.x));
		 int gridY = int(((mouse_pos.y - 100) / SqaureSize.y));
		 Cell *clicked_cell = get_cell(gridX,gridY);
		 if(!clicked_cell->Pressed){
		   clicked_cell->Pressed = true;
		   NonMinesLeft--;
		 }
		 if (clicked_cell->IsAMine){
		   RestartButton.Dead = true;
		 }else if(clicked_cell->MinesInArea == 0){
		   flood_fill(gridX, gridY);
		   //update_mine_counter();
		 }
	  }else if(mouse_pos.x <= (RestartButton.RestartRect.width + RestartButton.RestartRect.x) && mouse_pos.x >= (RestartButton.RestartRect.x)){
		if(mouse_pos.y <= (RestartButton.RestartRect.y + RestartButton.RestartRect.height) && mouse_pos.y >= RestartButton.RestartRect.y){
		  RestartButton.Pressed = true;
		  init_mine_grid();
		  RestartButton.Dead = false;
		  RestartButton.Won = false;
		}
	  }
    }
	if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
	  Vector2 mouse_pos = GetMousePosition();
	  if(!RestartButton.Dead && !RestartButton.Won && mouse_pos.y >= 100){
		int gridX = int((mouse_pos.x / SqaureSize.x));
		int gridY = int(((mouse_pos.y - 100) / SqaureSize.y));
		get_cell(gridX, gridY)->Flagged = !get_cell(gridX, gridY)->Flagged;
		
	  }
	}
	if(!NonMinesLeft){
	  RestartButton.Won = true;
	}
	render_game_state();
   	EndDrawing();
  }
  unload_assests();
  CloseWindow();
  return 0;
}

void render_game_state() {
  ClearBackground(BackGroundColor);
  const Rectangle TextureSource{
      .x = 0,
      .y = 0,
      .width = float(DefaultBoxTexture.width),
      .height = float(DefaultBoxTexture.height),
  };
  const Rectangle FlaggedTextureSource{
	.x = 0,
	.y = 0,
	.width = float(FlaggedCellTexture.width),
	.height = float(FlaggedCellTexture.height),
  };
  render_restart_button();
  for (int I = 0; I < GridRows; I++) {
    for (int i = 0; i < GridCols; i++) {
	  Cell current_cell = *get_cell(i, I);
          Rectangle concated_shape = {
              .x = current_cell.postion.x,
              .y = current_cell.postion.y,
              .width = SqaureSize.y,
              .height = SqaureSize.x,
          };
	 if (current_cell.Pressed){
	  handle_cell_texture(current_cell,TextureSource,concated_shape);
    } else {
	   if(current_cell.Flagged){
		DrawTexturePro(FlaggedCellTexture,FlaggedTextureSource,concated_shape,Vector2{0.0,0.0},0.0f,WHITE);
	   }else{
		 DrawTexturePro(DefaultBoxTexture, TextureSource, concated_shape, Vector2{0.0, 0.0}, 0.0f, WHITE);
	   }
	   
   }
	 //debug printing for mine board
	 // char debug_buffer[40];
	 //  if (current_cell.IsAMine) {
	 // 	DrawText("M",(current_cell.postion.x),(current_cell.postion.y),20,RED);
     //  } else {
     //    snprintf(debug_buffer, sizeof(debug_buffer), "%d",
     //                  current_cell.MinesInArea);
	 // 	DrawText(debug_buffer,(current_cell.postion.x),(current_cell.postion.y),20,GREEN);
     //  }
	 //  snprintf(debug_buffer, sizeof(debug_buffer), "Non Mines Remaing%d",NonMinesLeft);
	 //  DrawText(debug_buffer,0,0,20,RED);
    }
  }
}
// don't know if the inline keyword actually does anything on modern compilers?
// made this a seperate function so the render loop isn't as verbose
inline void handle_cell_texture(Cell current_cell,Rectangle TextureSource,Rectangle concated_shape) {
  if (current_cell.IsAMine) {
    Rectangle MineTextureSource {
      .x = 0, .y = 0,
	  .width = float(EvilMineCat.width),.height = float(EvilMineCat.height),
    };
    DrawTexturePro(EvilMineCat, MineTextureSource, concated_shape,
                   Vector2{0.0, 0.0}, 0.0f, WHITE);
	return;
  }
  if(!current_cell.MinesInArea){
	DrawTexturePro(PressedBoxTexture,TextureSource,concated_shape,Vector2{0.0f,0.0f},0.0f,WHITE);
	return;
  }
  Rectangle NumberTextureSource{
	.x = 0,
	.y = 0,
	.width = float(NumberAssests[current_cell.MinesInArea - 1].width),
	.height = float(NumberAssests[current_cell.MinesInArea - 1].height),
  };
  DrawTexturePro(NumberAssests[current_cell.MinesInArea - 1], NumberTextureSource, concated_shape, Vector2{0.0, 0.0}, 0.0f, WHITE);

}
inline void render_restart_button() {
    const Rectangle RestartButtonSource{
      .x = 0,
      .y = 0,
      .width = float(YellowCatAlive.width),
      .height = float(YellowCatAlive.height),
  	};
    if (RestartButton.Dead) {
      DrawTexturePro(YellowCatDead, RestartButtonSource,
                     RestartButton.RestartRect, Vector2{0.0f, 0.0f}, 0.0f,
                     WHITE);
	  return;
    }
	if(RestartButton.Won){
	 DrawTexturePro(YellowCatWon, RestartButtonSource,
                     RestartButton.RestartRect, Vector2{0.0f, 0.0f}, 0.0f,
                     WHITE);
	 return;
	}
	  
	if (RestartButton.Pressed){
	  DrawTexturePro(YellowCatAlivePressed, RestartButtonSource,
                     RestartButton.RestartRect, Vector2{0.0f, 0.0f}, 0.0f,
                     WHITE);
	}
	DrawTexturePro(YellowCatAlive, RestartButtonSource,
                         RestartButton.RestartRect, Vector2{0.0f, 0.0f}, 0.0f,
					 WHITE);
	
	
}
void load_assests() {
  char  number_path_buffer[200];
  DefaultBoxTexture = LoadTexture("./src/assests/defaultunpressed.png");
  PressedBoxTexture = LoadTexture("./src/assests/defaultpressed.png");
  EvilMineCat = LoadTexture("./src/assests/evilcatpressed.png");
  YellowCatAlive = LoadTexture("./src/assests/yellowcatun.png");
  YellowCatAlivePressed = LoadTexture("./src/assests/yellowcatpr.png");
  YellowCatDead = LoadTexture("./src/assests/deadyellowcat.png");
  YellowCatWon = LoadTexture("./src/assests/coolyellowcatun.png");
  FlaggedCellTexture = LoadTexture("./src/assests/evilcatcrossed.png");
  for(int i = 0; i < 8;i++){
	snprintf(number_path_buffer,sizeof(number_path_buffer),"./src/assests/%d.png",i + 1);
	NumberAssests[i] = LoadTexture(number_path_buffer);
  }
}
void unload_assests() {
  UnloadTexture(DefaultBoxTexture);
  UnloadTexture(PressedBoxTexture);
  UnloadTexture(EvilMineCat);
  UnloadTexture(YellowCatAlive);
  UnloadTexture(YellowCatAlivePressed);
  UnloadTexture(YellowCatDead);
  for(int i = 0;i < 8;i++){
	UnloadTexture(NumberAssests[i]);
  }
}


void flood_fill(int Sx, int Sy){
  const Vector2I dir_table[8]{
	{0,1},
	{0,-1},
	{1,0},
	{-1,0},
	{1,1},
	{-1,-1},
	{-1,1},
	{1,-1},
  };
  Dyn_Arry<Vector2I> cord_queue = new_dynarray<Vector2I>(10);
  flood_hashmap map = create_flood_map((GridCols * GridRows) + 10);
  int qp = 0;
  //int ql = 1;
  cord_queue.append({Sx,Sy});
  hash_add(cord_queue[0],&map);
  for(;qp < cord_queue.len;qp++){
	Vector2I current_cords = cord_queue[qp];
	Cell *adjacent_cell;	
	for(int i = 0; i < 8;i++){
	  Vector2I adjacent_vector = current_cords + dir_table[i];
	  if(adjacent_vector.x < 0 || adjacent_vector.y < 0 || adjacent_vector.x >= GridCols || adjacent_vector.y >= GridRows){
		continue;
	  }
	  adjacent_cell = get_cell(adjacent_vector.x, adjacent_vector.y);
	  if(!adjacent_cell->Pressed && !adjacent_cell->IsAMine){
		adjacent_cell->Pressed = true;
		NonMinesLeft--;
	  }
	  if(adjacent_cell->MinesInArea == 0){
		if(!hash_look(adjacent_vector, &map)){
		  cord_queue.append(adjacent_vector);
		  hash_add(adjacent_vector, &map);
		}
	  }
	}
  }
  destroy_flood_map(&map);
  cord_queue.free_arr();
}

// for now I am just brute forcing it until I come up with a decent hashing solution -- solved
// bool flood_check(Vector2I *queue, int ql, Vector2I cords) {
//   for (int i = 0; i < ql; i++) {
//     Vector2I comp_cord = queue[i];
//     if (cords.x == comp_cord.x && cords.y == comp_cord.y) {
// 	  return true;
//     }
//   }
//   return false;
// }
void update_mine_counter(){
  int non_mines_remaining = 0;
  for(int I = 0; I < 9;I++){
	for(int i = 0;i < 9;i++){
	  if(!get_cell(i, I)->Pressed && !get_cell(i, I)->IsAMine){
		non_mines_remaining++;
	  }
	}
  }
  NonMinesLeft = non_mines_remaining;
}
void generate_random_mines(int in,int total_mines,Vector2I *mine_array) {
  int counter = in;
  int RandRow = rand() % GridRows;
  int RandCol = rand() % GridCols;
  if (get_cell(RandCol, RandRow)->IsAMine) {
	return generate_random_mines(counter,total_mines,mine_array);
  }
  get_cell(RandCol, RandRow)->IsAMine = true;
  mine_array[counter] = {RandCol,RandRow};
  counter++;
  if (counter < total_mines) {
	return generate_random_mines(counter,total_mines,mine_array);
  }
  return;
}
Cell *get_cell(int x,int y){
  int row = y * GridCols;
  int pos = row + x;
  return &MineGrid[pos];
}
// I could of probably done this better -- i did end up doing it better
void calculate_mines_in_area(Vector2I *mine_array) {
  const Vector2I dir_table[8]{
	  {0,1},
	  {0,-1},
	  {1,0},
	  {-1,0},
	  {1,1},
	  {-1,-1},
	  {-1,1},
	  {1,-1},
  };
  for (int i = 0;i < TotalMines;i++) {
	Vector2I current_cords = mine_array[i];
	if(current_cords.x){
	   get_cell(current_cords.x - 1,current_cords.y)->MinesInArea++;
	}
	if(current_cords.y){
	  get_cell(current_cords.x,current_cords.y - 1)->MinesInArea++;  
	}
	if(current_cords.x < GridCols - 1){
	  get_cell(current_cords.x + 1,current_cords.y)->MinesInArea++;
	}
	if(current_cords.y < GridRows - 1){
	  get_cell(current_cords.x,current_cords.y + 1)->MinesInArea++; 
	}
	if(current_cords.x && current_cords.y){
	   get_cell(current_cords.x - 1,current_cords.y - 1)->MinesInArea++;
	}
	if(current_cords.x < GridCols - 1 && current_cords.y < GridRows - 1){
	  get_cell(current_cords.x + 1,current_cords.y + 1)->MinesInArea++;
	}
	if(current_cords.x && current_cords.y < GridRows - 1){
	  get_cell(current_cords.x - 1,current_cords.y + 1)->MinesInArea++;
	}
	if(current_cords.x < GridCols - 1 && current_cords.y){
	  get_cell(current_cords.x + 1, current_cords.y - 1)->MinesInArea++;
	}	
  }
}
void get_mine_size(){
  if(GameDif == BEGIN_MODE){
	GridCols = 9;
	GridRows = 9;
	TotalMines = BEGIN_MINE;
  }else if( GameDif == INTER_MODE){
	GridCols = 16;
	GridRows = 16;
	TotalMines = INTER_MINE;
  }else if(GameDif == EXPERT_MODE){
	GridCols = 30;
	GridRows = 16;
	TotalMines = EXPERT_MINE;
  }
  SqaureSize.x = float(GRID_WIDTH) / float(GridCols);
  SqaureSize.y = float(GRID_HEIGHT) / float(GridRows);
}
void init_mine_grid() {
  float X = 0;
  float Y = GRID_START;
  get_mine_size();
  if(MineGrid != NULL){
	free(MineGrid);
    MineGrid = NULL;
  }
  MineGrid = (Cell*)calloc(GridCols * GridRows,sizeof(*MineGrid));
  NonMinesLeft = (GridCols * GridRows) - TotalMines;
  for (int I = 0; I < GridRows; I++) {
    for (int i = 0; i < GridCols; i++) {
      get_cell(i, I)->postion = Vector2{.x = float(X), .y = float(Y)};
	  get_cell(i, I)->Pressed = false;
	  get_cell(i, I)->IsAMine = false;
	  get_cell(i, I)->MinesInArea = 0;
	  X += SqaureSize.x;
    }
    X = 0;
	Y += SqaureSize.y;
  }
  srand(time(0));
  Vector2I *mine_array = (Vector2I *)calloc(TotalMines,sizeof(Vector2I));
  generate_random_mines(0,TotalMines,mine_array);
  calculate_mines_in_area(mine_array);
  free(mine_array);
} 
