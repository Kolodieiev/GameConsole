from PIL import Image

def convert_to_black_and_white(input_path, output_path):
    img = Image.open(input_path).convert("L")  # Конвертуємо в градації сірого
    pixels = img.load()

    threshold = 10

    for y in range(img.height):
        for x in range(img.width):
            pixels[x, y] = 255 if pixels[x, y] >= threshold else 0

    img.save(output_path)

convert_to_black_and_white("GC_Front.png", "output.png")
