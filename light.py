import random

map_lines = [
"44444444444",
"4E........4",
"444.44444.4",
"4...4.4...4",
"4.4.4.44444",
"4.4.......4",
"4.4.4444444",
"4.4.....4.4",
"4.4.4.444.4",
"4.4.4.....4",
"44444444444"
]

# Generate the light placement lines
light_lines = []
i = 0
for y, row in enumerate(map_lines):
	for x, char in enumerate(row):
		if char == '.':
			i+=1
			if (i % 4 == 0):
				r, g, b = random.randint(128, 255), random.randint(128, 255), random.randint(128, 255)
				light_lines.append(f"L (({x + 0.5}, {y + 0.5}), {{{r}, {g}, {b}}}, 1)")

# Write to a text file
output_path = "lights_for_dot_tiles.txt"
with open(output_path, "w") as f:
	f.write("\n".join(light_lines))