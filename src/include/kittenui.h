#ifndef KITTEN_UI_H
#define KITTENUI_H
struct DropDownBox{
  Rectangle Body;
  //  Rectangle OptionsBox[3];
  char Options[3][15];
  Vector2 OptionPos[3];
  Vector2 LinePos[3];
  bool showing = false;
};
DropDownBox init_box();
void draw_drop_down_box(DropDownBox *box);

#endif
