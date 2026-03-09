from PIL import Image  

img = Image.open("snake-graphics.png")

tile_w = 64
tile_h = 64

columns = img.width // tile_w
rows = img.height // tile_h

index = 0

for y in range(rows):
    for x in range(columns):
        left = x * tile_w
        top = y*tile_h
        right = left + tile_w
        bottom = top + tile_h

        tile = img.crop((left, top, right, bottom))
        tile.save(f"Snake_tile_{index}.png")
        index += 1