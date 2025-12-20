#ifndef FLOOD_HASH_H
#define FLOOD_HASH_H

struct Vector2I{
  int x,y;
  bool operator==(Vector2I other){
	return (this->x == other.x && this->y == other.y);
  }
  void operator+=(Vector2I other){
	this->x += other.x;
	this->y += other.y;
  }
  Vector2I operator+(Vector2I other){
	Vector2I ret = *this;
	ret.x += other.x;
	ret.y += other.y;
	return ret;
  }
};

struct hash_bucket{
  bool occuiped = false;
  Vector2I cord;
};

struct flood_hashmap{
  hash_bucket *buffer;
  int cap;
};

flood_hashmap create_flood_map(int cap);

void destroy_flood_map(flood_hashmap *map);

void hash_add(Vector2I cords,flood_hashmap *map);

bool hash_look(Vector2I cords,flood_hashmap *map);



#endif
