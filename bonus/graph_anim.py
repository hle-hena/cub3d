import re
import matplotlib.pyplot as plt
from matplotlib.patches import Arc, Circle
from matplotlib.widgets import Button
import random
import math

# Extended color palette
COLOR_PALETTE = [
    (0.894, 0.102, 0.110),  # red
    (0.215, 0.494, 0.721),  # blue
    (0.302, 0.686, 0.290),  # green
    (1.000, 0.498, 0.000),  # orange
    (0.596, 0.306, 0.639),  # purple
    (1.000, 1.000, 0.200),  # yellow
    (0.651, 0.337, 0.157),  # brown
    (0.969, 0.506, 0.749),  # pink
    (0.6, 0.6, 0.6),        # gray
    (0.3, 0.3, 0.3),        # dark gray
    (0.4, 0.760, 0.647),    # turquoise
    (0.988, 0.553, 0.349),  # salmon
]

def parse_graphs(input_text):
    graphs_raw = input_text.strip().split("This is the graph of")
    graphs = []
    letters = []
    for raw in graphs_raw[1:]:
        first_line = raw.strip().split('\n')[0]
        m_letter = re.match(r"([A-Za-z])", first_line)
        letter = m_letter.group(1) if m_letter else '?'
        letters.append(letter)

        lines = []
        arcs = []
        for line in raw.strip().split('\n'):
            line = line.strip()
            m_line = re.match(r"A line is connecting \{([-\d.e]+), ([-\d.e]+)\} to \{([-\d.e]+), ([-\d.e]+)\}", line)
            if m_line:
                x1, y1, x2, y2 = map(float, m_line.groups())
                lines.append(((x1, y1), (x2, y2)))
                continue
            m_arc = re.match(
                r"An arc is connecting \{([-\d.e]+), ([-\d.e]+)\} to \{([-\d.e]+), ([-\d.e]+)\}, with \{([-\d.e]+), ([-\d.e]+)\} as center",
                line)
            if m_arc:
                x1, y1, x2, y2, cx, cy = map(float, m_arc.groups())
                arcs.append(((x1, y1), (x2, y2), (cx, cy)))
                continue
        graphs.append({'lines': lines, 'arcs': arcs})
    return graphs, letters

def angle_between(center, point):
    return math.degrees(math.atan2(point[1] - center[1], point[0] - center[0]))

def cycle_colors(n):
    colors = []
    palette_size = len(COLOR_PALETTE)
    prev_idx = -1
    for i in range(n):
        idx = (prev_idx + 1) % palette_size
        colors.append(COLOR_PALETTE[idx])
        prev_idx = idx
    return colors

def draw_graph(ax, graph, letter, previous_points):
    ax.clear()
    ax.set_aspect('equal')
    ax.set_xlim(-0.1, 1.1)
    ax.set_ylim(-0.1, 1.1)
    ax.set_xticks([])
    ax.set_yticks([])

    nb_links = len(graph['lines']) + len(graph['arcs'])
    ax.set_title(f"Graph of {letter} ({nb_links} links)")

    total_edges = nb_links
    colors = cycle_colors(total_edges)

    # Track all points in this graph
    current_points = set()

    # Draw lines
    for i, (p1, p2) in enumerate(graph['lines']):
        ax.plot([p1[0], p2[0]], [p1[1], p2[1]], color=colors[i], linewidth=2)
        current_points.add(p1)
        current_points.add(p2)

    # Draw arcs
    for j, (p1, p2, center) in enumerate(graph['arcs']):
        idx = len(graph['lines']) + j
        color = colors[idx]

        start_angle = angle_between(center, p1)
        end_angle = angle_between(center, p2)
        angle_diff = (end_angle - start_angle) % 360
        radius = math.dist(center, p1)

        if p1 == p2:
            circle = Circle(center, radius, edgecolor=color, fill=False, linestyle='solid', linewidth=2)
            ax.add_patch(circle)
            current_points.add(center)
        else:
            if angle_diff == 0:
                angle_diff = 360
            arc = Arc(center, 2*radius, 2*radius,
                      angle=0,
                      theta1=start_angle,
                      theta2=start_angle + angle_diff,
                      color=color,
                      linewidth=2)
            ax.add_patch(arc)
            current_points.add(p1)
            current_points.add(p2)

    # Highlight new points in red, others in black
    new_points = current_points - previous_points
    old_points = current_points & previous_points

    if old_points:
        xs_old, ys_old = zip(*old_points)
        ax.scatter(xs_old, ys_old, color='black', s=40, zorder=5)
    if new_points:
        xs_new, ys_new = zip(*new_points)
        ax.scatter(xs_new, ys_new, color='red', s=40, zorder=6)

    return current_points

# === Main program ===
fig, ax = plt.subplots(figsize=(6,6))
plt.subplots_adjust(bottom=0.2)

axprev = plt.axes([0.3, 0.05, 0.1, 0.075])
axnext = plt.axes([0.6, 0.05, 0.1, 0.075])
btn_prev = Button(axprev, 'Previous')
btn_next = Button(axnext, 'Next')

with open("input.txt", "r") as f:
    input_text = f.read()

graphs, letters = parse_graphs(input_text)
current_index = 0
previous_points = set()

def update():
    global previous_points
    previous_points = draw_graph(ax, graphs[current_index], letters[current_index], previous_points)
    fig.canvas.draw_idle()

def prev(event):
    global current_index, previous_points
    if current_index > 0:
        current_index -= 1
        previous_points = set()  # Reset to avoid false "new points"
        update()

def next(event):
    global current_index
    if current_index < len(graphs) - 1:
        current_index += 1
        update()

btn_prev.on_clicked(prev)
btn_next.on_clicked(next)

update()
plt.show()
