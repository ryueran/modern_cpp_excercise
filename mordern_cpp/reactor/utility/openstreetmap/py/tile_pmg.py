import os
import math
import requests
from PIL import Image

# 下载瓦片的 URL 模板
TILE_URL = "http://localhost/osm/{z}/{x}/{y}.png"
SAVE_DIR = "osm_tiles"

# 创建保存目录
os.makedirs(SAVE_DIR, exist_ok=True)

def latlon_to_tile(lat, lon, zoom):
    """ 将经纬度转换为瓦片 x, y 编号 """
    n = 2 ** zoom
    x = int((lon + 180.0) / 360.0 * n)
    y = int((1.0 - math.log(math.tan(math.radians(lat)) + 1 / math.cos(math.radians(lat))) / math.pi) / 2.0 * n)
    return x, y

def download_tile(z, x, y):
    """ 下载单个瓦片 """
    url = TILE_URL.format(z=z, x=x, y=y)
    tile_path = os.path.join(SAVE_DIR, f"{z}_{x}_{y}.png")
    
    if not os.path.exists(tile_path):  # 避免重复下载
        response = requests.get(url, stream=True)
        if response.status_code == 200:
            with open(tile_path, 'wb') as f:
                for chunk in response.iter_content(1024):
                    f.write(chunk)
            print(f"下载: {tile_path}")
        else:
            print(f"无法下载: {tile_path}")
    return tile_path

def stitch_tiles(zoom, x_min, x_max, y_min, y_max):
    """ 拼接瓦片为大地图 """
    tile_size = 256  # 每个瓦片 256x256 像素
    width = (x_max - x_min + 1) * tile_size
    height = (y_max - y_min + 1) * tile_size
    map_img = Image.new("RGB", (width, height))

    for x in range(x_min, x_max + 1):
        for y in range(y_min, y_max + 1):
            tile_path = os.path.join(SAVE_DIR, f"{zoom}_{x}_{y}.png")
            if os.path.exists(tile_path):
                tile = Image.open(tile_path)
                px = (x - x_min) * tile_size
                py = (y - y_min) * tile_size
                map_img.paste(tile, (px, py))

    output_path = f"stitched_map_zoom{zoom}.png"
    map_img.save(output_path)
    print(f"地图已拼接: {output_path}")

def download_and_stitch_map(lat1, lon1, lat2, lon2, zoom_levels):
    """ 下载并拼接多个 zoom 级别的瓦片 """
    for zoom in zoom_levels:
        x_min, y_min = latlon_to_tile(lat1, lon1, zoom)
        x_max, y_max = latlon_to_tile(lat2, lon2, zoom)
        
        # 依次下载瓦片
        for x in range(x_min, x_max + 1):
            for y in range(y_min, y_max + 1):
                download_tile(zoom, x, y)

        # 拼接地图
        stitch_tiles(zoom, x_min, x_max, y_min, y_max)

# 示例: 下载北京某区域 zoom 12-14 的地图
download_and_stitch_map(39.96, 116.30, 39.90, 116.40, zoom_levels=[0])
