#include <cstdlib>
#include <cstdint>
struct Vector2I{
  int x,y;
  bool operator==(Vector2I other){
	return (this->x == other.x && this->y == other.y);
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

flood_hashmap create_flood_map(int cap){
  flood_hashmap ret;
  ret.buffer = (hash_bucket *)calloc(cap,sizeof(hash_bucket));
  ret.cap = cap;
  return ret;
}

void destroy_flood_map(flood_hashmap *map){
  free(map->buffer);
  map->cap = -1;
}

uint64_t hash_func(Vector2I cords){
  uint64_t x = cords.x;
  uint64_t y = cords.y;
  uint64_t hash = ((x << 32) | y);
  return hash;
}

void hash_add(Vector2I cords,flood_hashmap *map){
  int index = hash_func(cords) % map->cap;
  if(map->buffer[index].occuiped){
	for(;;){
	  index = (index + 1) % map->cap;
	  if(!map->buffer[index].occuiped){
		return;
	  }
	}
  }
  map->buffer[index].cord = cords;
}

bool hash_look(Vector2I cords,flood_hashmap *map){
  int index = hash_func(cords) % map->cap;
  if(map->buffer[index].cord == cords){
	return true;
  }
  for(;;){
	index = (index + 1) % map->cap;
	if(map->buffer[index].cord == cords){
	  return true;
	}else if(!map->buffer[index].occuiped){
	  break;
	}
  }
  return false;
}
