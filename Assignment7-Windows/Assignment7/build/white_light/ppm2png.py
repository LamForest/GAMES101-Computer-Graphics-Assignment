import os
from os import listdir
from os.path import isfile, join
from PIL import Image

mypath = "."
output_dir = "png"

onlyppms = [f for f in listdir(mypath) if isfile(join(mypath, f)) and f.endswith(".ppm")]

for f in onlyppms:
    im = Image.open(f)
    im.save(os.path.join(output_dir, f[:-4] + ".png"))