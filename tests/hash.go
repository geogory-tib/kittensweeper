package main

import (
	"fmt"
	"math/rand"
)

type Vector2I struct{
	x,y int
}

func(v Vector2I)same(v2 Vector2I) bool{
	if(v.x == v2.x && v.y == v2.y){
		return true
	}
	return false
}
func hash_func(x,y,length int) int{
	X2 := (x + y * 4 + 22 * length *  4) - 48
	Y2 := (y + x * 10 + 42 * length * 420) -49
	hash := (X2 + Y2 * 22 * 69 + 22 + 48 +2121) % length
	return hash
}


func main(){
	m := make(map[int]Vector2I)
	colls := 0
	inputs := 0
	same_cords := 0
	for  inputs = range 700{
		vec := Vector2I{}
		vec.x = rand.Int() % 9
		vec.y = rand.Int() % 9
		hsh := hash_func(vec.x,vec.y,200)
		fmt.Printf("Inputs %v, hash: hsh %d\n",vec,hsh)
		vec_test,ok := m[hsh]
		if(!ok){
			m[hsh] = vec
		}else if(vec.same(vec_test)){
			fmt.Printf("Vec %v, Vec_Test %v ", vec,vec_test)
			fmt.Println("Same Cord")
			same_cords++
		}else{
			colls++
			fmt.Printf("Vec %v Vec_Test %v ",vec,vec_test)
			fmt.Println("Collison")
		}
	}
	fmt.Printf("out of %d unique  inputs there were %d\n",inputs -same_cords,colls)
}
