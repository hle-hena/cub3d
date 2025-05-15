import re

input_data = """Tried for {1, 1}, with ray_dir {-0.428, -1.000} and start hit at {1.43, 1.00}
Tried for {1, 1}, with ray_dir {-0.428, -1.000} and start hit at {1.43, 1.00}
Tried for {1, 1}, with ray_dir {-0.427, -1.000} and start hit at {1.43, 1.00}
Tried for {1, 1}, with ray_dir {-0.426, -1.000} and start hit at {1.43, 1.00}
Tried for {1, 1}, with ray_dir {-0.426, -1.000} and start hit at {1.44, 1.00}
Tried for {1, 1}, with ray_dir {-0.425, -1.000} and start hit at {1.44, 1.00}
Tried for {1, 1}, with ray_dir {-0.425, -1.000} and start hit at {1.44, 1.00}
Tried for {1, 1}, with ray_dir {-0.424, -1.000} and start hit at {1.44, 1.00}
Tried for {1, 1}, with ray_dir {-0.423, -1.000} and start hit at {1.44, 1.00}
Tried for {1, 1}, with ray_dir {-0.423, -1.000} and start hit at {1.44, 1.00}
Tried for {1, 1}, with ray_dir {-0.422, -1.000} and start hit at {1.44, 1.00}
Tried for {1, 1}, with ray_dir {-0.422, -1.000} and start hit at {1.45, 1.00}
Tried for {1, 1}, with ray_dir {-0.421, -1.000} and start hit at {1.45, 1.00}
Tried for {1, 1}, with ray_dir {-0.420, -1.000} and start hit at {1.45, 1.00}
Tried for {1, 1}, with ray_dir {-0.420, -1.000} and start hit at {1.45, 1.00}
Tried for {1, 1}, with ray_dir {-0.419, -1.000} and start hit at {1.45, 1.00}
Tried for {1, 1}, with ray_dir {-0.419, -1.000} and start hit at {1.45, 1.00}
Tried for {1, 1}, with ray_dir {-0.418, -1.000} and start hit at {1.46, 1.00}
Tried for {1, 1}, with ray_dir {-0.417, -1.000} and start hit at {1.46, 1.00}
Tried for {1, 1}, with ray_dir {-0.417, -1.000} and start hit at {1.46, 1.00}"""

pattern = re.compile(r"ray_dir\s*\{(-?[\d.]+),\s*(-?[\d.]+)}.*start hit at\s*\{([\d.]+),\s*([\d.]+)}")
commands = []

for i, match in enumerate(pattern.finditer(input_data)):
    dx, dy, sx, sy = map(float, match.groups())
    ex = sx + dx
    ey = sy + dy
    commands.append(f"Vector(({sx:.3f}, {sy:.3f}), ({ex:.3f}, {ey:.3f}))")

for cmd in commands:
    print(cmd)
