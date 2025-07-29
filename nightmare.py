import sys
import os

def tile_xpm_2x2(input_path, output_path):
    with open(input_path, 'r') as f:
        lines = f.readlines()

    # Find the header line
    header_line_index = next(i for i, line in enumerate(lines) if '"' in line)
    header_parts = lines[header_line_index].split('"')[1].split()
    width, height, num_colors, chars_per_pixel = map(int, header_parts)

    # Extract color and pixel lines
    color_lines = lines[header_line_index + 1 : header_line_index + 1 + num_colors]
    pixel_lines = lines[header_line_index + 1 + num_colors : header_line_index + 1 + num_colors + height]

    # Duplicate each line horizontally
    pixel_lines_horiz = [
        '"' + line.strip().strip('"') * 2 + '",\n' for line in pixel_lines
    ]

    # Duplicate lines vertically
    pixel_lines_2x2 = pixel_lines_horiz + pixel_lines_horiz

    # Update header
    new_width = width * 2
    new_height = height * 2
    new_header = f'"{new_width} {new_height} {num_colors} {chars_per_pixel}",\n'

    # Write output file
    with open(output_path, 'w') as f:
        # Copy everything before header
        for i in range(header_line_index):
            f.write(lines[i])
        f.write(new_header)
        f.writelines(color_lines)
        f.writelines(pixel_lines_2x2)
        # Append closing brace if it exists
        if lines[-1].strip() == "};":
            f.write("};\n")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 algo.py path/to/image.xpm")
        sys.exit(1)

    input_file = sys.argv[1]
    base, _ = os.path.splitext(input_file)
    output_file = f"{base}x2_2.xpm"

    tile_xpm_2x2(input_file, output_file)
    print(f"Tiled image saved as {output_file}")
