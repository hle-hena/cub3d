import re
import random
import matplotlib.pyplot as plt
import numpy as np
from collections import namedtuple
import colorsys

Vec = namedtuple("Vec", ["x", "y"])
Link = namedtuple("Link", ["start", "end", "type", "center"])

def parse_vec(text):
    nums = list(map(float, re.findall(r"[-+]?\d*\.\d+|\d+", text)))
    return Vec(nums[0], nums[1])

def parse_graph_section(section):
    lines = section.strip().splitlines()
    if not lines:
        return None, [], []

    header = lines[0].strip()
    if not header.startswith("This is the graph of"):
        return None, [], []

    char = header.split("This is the graph of")[1].strip(": \n")
    nodes = set()
    links = []

    for line in lines[1:]:
        if "line is connecting" in line:
            start, end = re.findall(r"\{[^}]+\}", line)
            v1, v2 = parse_vec(start), parse_vec(end)
            nodes.update([v1, v2])
            links.append(Link(v1, v2, "line", None))
        elif "arc is connecting" in line:
            start, end, center = re.findall(r"\{[^}]+\}", line)
            v1, v2, c = parse_vec(start), parse_vec(end), parse_vec(center)
            nodes.update([v1, v2])
            links.append(Link(v1, v2, "arc", c))

    return char, list(nodes), links

def draw_arc(ax, start, end, center, color):
    radius = np.hypot(center.x - start.x, center.y - start.y)

    def angle(p):
        return np.arctan2(p.y - center.y, p.x - center.x)

    if start == end:
        theta = np.linspace(0, 2 * np.pi, 200)
    else:
        theta1 = angle(start)
        theta2 = angle(end)
        if theta2 < theta1:
            theta2 += 2 * np.pi
        theta = np.linspace(theta1, theta2, 100)

    x = center.x + radius * np.cos(theta)
    y = center.y + radius * np.sin(theta)
    ax.plot(x, y, '--', color=color, linewidth=2)

def generate_distinct_colors(n):
    hues = [i / n for i in range(n)]
    random.shuffle(hues)
    return [colorsys.hsv_to_rgb(h, 0.8, 0.9) for h in hues]

def draw_all_graphs(graphs):
    n = len(graphs)
    cols = min(n, 3)
    rows = (n + cols - 1) // cols

    fig, axes = plt.subplots(rows, cols, figsize=(5 * cols, 5 * rows))
    if n == 1:
        axes = [[axes]]
    elif rows == 1:
        axes = [axes]
    elif cols == 1:
        axes = [[ax] for ax in axes]

    for idx, (char, nodes, links) in enumerate(graphs):
        r, c = divmod(idx, cols)
        ax = axes[r][c]
        ax.set_aspect('equal')
        ax.set_title(f"Tile '{char}' ({len(links)} links)")
        ax.set_xlabel("X")
        ax.set_ylabel("Y")

        for node in nodes:
            ax.plot(node.x, node.y, 'ko')
            ax.text(node.x, node.y + 0.01, f"({node.x:.2f}, {node.y:.2f})", fontsize=6, ha='center', color='blue')

        colors = generate_distinct_colors(len(links))
        for link, color in zip(links, colors):
            if link.type == 'line':
                ax.plot([link.start.x, link.end.x], [link.start.y, link.end.y], '-', color=color, linewidth=2)
            elif link.type == 'arc':
                draw_arc(ax, link.start, link.end, link.center, color=color)

        ax.grid(True)

    # Hide any unused subplots
    for i in range(n, rows * cols):
        r, c = divmod(i, cols)
        axes[r][c].axis('off')

    plt.tight_layout()
    plt.show()

with open("input.txt", "r") as f:
    input_text = f.read()

# Parse sections and draw them all
sections = re.split(r"\n(?=This is the graph of)", input_text.strip().split("Doing")[0])
graphs = []

for section in sections:
    char, nodes, links = parse_graph_section(section)
    if char is not None:
        graphs.append((char, nodes, links))

draw_all_graphs(graphs)