import math

def generate_circle_segments(radius=0.5, center=(0.5, 0.5), angle_step=10):
    segments = []
    cx, cy = center

    for angle in range(0, 360, angle_step):
        angle1 = math.radians(angle)
        angle2 = math.radians(angle + angle_step)

        x1 = round(cx + radius * math.cos(angle1), 4)
        y1 = round(cy + radius * math.sin(angle1), 4)
        x2 = round(cx + radius * math.cos(angle2), 4)
        y2 = round(cy + radius * math.sin(angle2), 4)

        segments.append(f"\twl (({x1}, {y1}), ({x2}, {y2})), 1")

    return segments

# Customize your parameters here
angle_step = 1  # degrees between points
segments = generate_circle_segments(radius=0.5, center=(0.5, 0.5), angle_step=angle_step)

# Output the result
print("\n".join(segments))