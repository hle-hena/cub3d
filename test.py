import re

# Your raw log input
log_data = """Ray_dir {-0.428, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.428, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.427, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.426, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.426, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.425, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.425, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.424, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.423, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.423, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.422, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.422, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.421, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.420, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.420, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.419, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.419, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.418, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.417, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.417, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.416, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.416, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.415, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.414, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.414, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.413, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.413, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.412, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.411, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.411, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.410, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.410, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.409, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.408, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.408, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.407, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.407, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.406, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.405, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.405, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.404, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.404, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.403, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.402, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.402, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.401, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.401, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.400, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.399, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.399, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.398, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.398, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.397, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.396, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.396, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.395, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.395, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.394, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.393, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.393, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.392, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.392, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.391, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.390, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.390, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.389, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.389, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.388, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.387, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.387, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.386, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.386, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.385, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.384, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.384, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.383, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.382, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.382, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.381, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.381, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.380, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.379, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.379, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.378, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.378, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.377, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.376, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.376, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.375, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.375, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.374, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.373, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.373, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.372, -1.000} and start at {2.500, 3.500}
Ray_dir {-0.372, -1.000} and start at {2.500, 3.500}
"""

# Extract vector data using regex
pattern = r"Ray_dir \{([-.\d]+), ([-.\d]+)\} and start at \{([-.\d]+), ([-.\d]+)\}"
matches = re.findall(pattern, log_data)

# Optional: length of the vector drawn in GeoGebra
length = 1.0

# Generate GeoGebra vector commands
for dx, dy, x, y in matches:
    x, y, dx, dy = float(x), float(y), float(dx), float(dy)
    y = -y
    dy = -dy
    end_x = x + dx * length
    end_y = y + dy * length
    print(f"Vector(({x:.3f}, {y:.3f}), ({end_x:.3f}, {end_y:.3f}))")
