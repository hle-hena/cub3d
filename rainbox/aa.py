import os
import random

img = []
for file in os.listdir():
    if (file.endswith(".xpm")):
        img.append(file)

tokens = list("`1234567890-=~!@#$%^&*_+qwertyuiop\\QRTYUIOPasdfghjkl;'DFGHJKL\"|zxcvbnm,/ZXCVBAM<>?" + ".")

used_tokens = []

random.shuffle(img)
ids = ["NO", "SO", "WE", "EA"]

for i in range(len(img) // 4):
    if (len(tokens) == 0):
        break ;
    cur_token = tokens.pop()
    used_tokens.append(cur_token)
    print(f"{cur_token} ", end="{\n")
    for id in ids:
        print(f"\t{id} textures/rainbow/{img.pop()}")
    print(f"\tC textures/wolfenstein/wood.xpm\n\tF textures/wolfenstein/grey_stone.xpm\n\tW {1 - (cur_token == '.')}\n", end="}\n\n")

extras = """L ((1.5, 1.5), {255, 0, 0}, 0.8)
L ((2.5, 2.5), 0.5)
L ((4.5, 6.5), {255, 0, 0}, 0.8)
L ((5.5, 7.5), 0.2)
L ((7.5, 7.5), {180, 0, 0}, 0.6)
L ((1.1, 8.1), {255, 0, 0}, 0.4)
L ((12.5, 5.5), {80, 0, 0}, 0.4)
L ((9.5, 4.5), {50, 0, 0}, 0.8)
L ((14.5, 5.5), {255, 0, 0}, 0.5)
L ((17.5, 1.5), {0, 54, 46}, 0.8)
P=.
"""
print(extras)

maze = """
AAAAAAAAAAAAAAAAAAA
AN.A..A..A..A..A..A
A..A..A..A..A..A..A
A..A..A..A..A..A..A
A..A..A.....A..A..A
A.................A
A....AAAAAAAA.....A
AAAA...........AAAA
A......A.A.A......A
AAAAAAAAAAAAAAAAAAA
"""

for c in maze:
    if (c != 'A'):
        print(c, end="")
    else:
        print(random.choice(used_tokens), end="")
