import random as r 
def hash_function(x,y,length):
    x2 = x + 2
    y2 = y + 2
    hsh = x2 + y2
    return hsh % length

def main():
    for x in range(100):
        x = r.randint(0,8)
        y = r.randint(0,8)
        print(f"X:{x} Y:{y}")
        hsh = hash_function(x,y,71)
        print(hsh)

main()
