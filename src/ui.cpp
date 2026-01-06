#include <cstdio>
#include <cstring>
#include "include/raylib.h"
#define WINDOW_WIDTH 486

#define WINDOW_HEIGH 586
#define DropDownBoxH (float(WINDOW_HEIGH) * 0.25f)
#define DropDownBoxW (float(WINDOW_WIDTH) * 0.25f + 10.f)
#define DropwDownBoxX (float(WINDOW_WIDTH) / 2.0f + 25)
#define DropwDownBoxY 50.0f
#define OutlineWidth (DropDownBoxW * 1.05f)
#define OutlineHeight (DropDownBoxH * 1.05f)
//#define OutlineX  ()
//#define OutlineY  (DropwDownBoxY * 0.90f)
#define LINESPACE 48.83f 
const Color BodyColor{
  .r = 192,
  .g = 192,
  .b = 192,
  .a = 255,
};
const Color OutlineColor{
  .r = 128,
  .g = 128,
  .b = 128,
  .a = 128,
};

struct DropDownBox{
  Rectangle Body;
  Rectangle Outline;
  //  Rectangle OptionsBox[3];
  char Options[3][15];
  Vector2 OptionPos[3];
  Vector2 LinePos[3];
  bool showing = false;
};

DropDownBox init_box(){
  
  DropDownBox ret;
  ret.Body.height = DropDownBoxH;
  ret.Body.width = DropDownBoxW;
  ret.Body.x = DropwDownBoxX;
  ret.Body.y = DropwDownBoxY;
  float OutlineX = DropwDownBoxX - ((OutlineWidth - DropDownBoxW) / 2.0f);
  float OutlineY = DropwDownBoxY - ((OutlineHeight - DropDownBoxH) / 2.0f);
  ret.Outline = {.x = OutlineX,.y = OutlineY,.width = OutlineWidth,.height = OutlineHeight};
  Vector2 StartPos{};
  StartPos.x = ret.Body.x;
  StartPos.y = DropwDownBoxY + LINESPACE;
  for(int i = 0; i < 3; i++){
	ret.LinePos[i] = StartPos;
	StartPos.y += LINESPACE;
  }
  strncpy(ret.Options[0], "Beginner", sizeof(ret.Options[0]));
  strncpy(ret.Options[1],"Intermediate",sizeof(ret.Options[1]));
  strncpy(ret.Options[2],"Expert",sizeof(ret.Options[2]));
  StartPos = {.x = DropwDownBoxX + 2,.y = (DropwDownBoxY + (LINESPACE / 2.0f))};
  for(int i = 0;i < 3;i++){
	ret.OptionPos[i] = StartPos;
	StartPos.y += LINESPACE;
  }
  return ret;
}

void draw_drop_down_box(DropDownBox *box){
  DrawRectangleRec(box->Outline, OutlineColor);
  DrawRectangleRec(box->Body,BodyColor);
  Vector2 EndOfLine{(box->Body.x + box->Body.width),0};
  for(int i = 0; i < 3; i++){
	EndOfLine.y = box->LinePos[i].y;
	DrawLineEx(box->LinePos[i], EndOfLine, 2.5f, OutlineColor);
	//	DrawLineV(box->LinePos[i],EndOfLine,OutlineColor);
  }
  for(int i = 0; i < 3; i++){
	DrawText(box->Options[i], box->OptionPos[i].x, box->OptionPos[i].y, 20, WHITE);
  }
}
