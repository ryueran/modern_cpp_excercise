import math
import requests
import os

def deg2num(lat_deg, lon_deg, zoom):
    lat_rad = math.radians(lat_deg)
    n = 2.0 ** zoom
    xtile = int((lon_deg + 180.0) / 360.0 * n)
    ytile = int((1.0 - math.log(math.tan(lat_rad) + (1 / math.cos(lat_rad))) / math.pi) / 2.0 * n)
    return (xtile, ytile)

def generate_tile_url(x, y, zoom, base_url):
    return f"{base_url}/{zoom}/{x}/{y}.png"

def download_tile(url, save_path):
    response = requests.get(url)
    if response.status_code == 200:
        with open(save_path, 'wb') as f:
            f.write(response.content)
    else:
        print(f"Failed to download {url}")

def download_tiles_in_range(top_left, bottom_right, zoom_range, base_url, output_dir):
    for zoom in range(zoom_range[0], zoom_range[1] + 1):
        x_start, y_start = deg2num(top_left[0], top_left[1], zoom)
        x_end, y_end = deg2num(bottom_right[0], bottom_right[1], zoom)
        
        for x in range(x_start, x_end + 1):
            for y in range(y_start, y_end + 1):
                url = generate_tile_url(x, y, zoom, base_url)
                save_path = os.path.join(output_dir, f"{zoom}_{x}_{y}.png")
                print(save_path)
                download_tile(url, save_path)
                print(f"Downloaded {save_path}")

# 示例使用
top_left = (8.9887, -79.5228)  # 纽约市的左上角经纬度
bottom_right = (8.9852, -79.5183)  # 纽约市的右下角经纬度
zoom_range = (14, 15)  # 缩放级别范围
base_url = "http://localhost/osm"
output_dir = "/home/muzi/Documents/exer_repo_cpp/mordern_cpp/reactor/utility/openstreetmap/py/tiles"

download_tiles_in_range(top_left, bottom_right, zoom_range, base_url, output_dir)