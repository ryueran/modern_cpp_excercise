import math
import requests
import os
from PIL import Image

TILE_SIZE = 256  # 瓦片尺寸（OSM 默认 256x256）
scale_factor = 5  # 放大最终图片 2 倍，提高分辨率

def deg2num(lat_deg, lon_deg, zoom):
    """将经纬度转换为瓦片坐标"""
    lat_rad = math.radians(lat_deg)
    n = 2.0 ** zoom
    xtile = math.floor((lon_deg + 180.0) / 360.0 * n)
    ytile = math.floor((1.0 - math.log(math.tan(lat_rad) + 1 / math.cos(lat_rad)) / math.pi) / 2.0 * n)
    return xtile, ytile

def num2deg(xtile, ytile, zoom):
    """计算瓦片左上角的经纬度"""
    n = 2.0 ** zoom
    lon_deg = xtile / n * 360.0 - 180.0
    lat_rad = math.atan(math.sinh(math.pi * (1 - 2 * ytile / n)))
    lat_deg = math.degrees(lat_rad)
    return lat_deg, lon_deg

def generate_tile_url(x, y, zoom, base_url):
    """生成 OSM 瓦片 URL"""
    return f"{base_url}/{zoom}/{x}/{y}.png"

def download_tile(url, save_path):
    """下载瓦片"""
    response = requests.get(url, timeout=5)
    if response.status_code == 200:
        with open(save_path, 'wb') as f:
            f.write(response.content)
    else:
        print(f"Failed to download {url}")

def download_tiles_in_range(top_left, bottom_right, zoom, base_url, output_dir):
    """下载包含 top_left 和 bottom_right 的所有瓦片"""
    os.makedirs(output_dir, exist_ok=True)

    x_start, y_start = deg2num(top_left[0], top_left[1], zoom)
    x_end, y_end = deg2num(bottom_right[0], bottom_right[1], zoom)

    tile_paths = {}  # 存储下载的瓦片路径
    for x in range(x_start, x_end + 1):
        for y in range(y_start, y_end + 1):
            url = generate_tile_url(x, y, zoom, base_url)
            save_path = os.path.join(output_dir, f"{zoom}_{x}_{y}.png")
            download_tile(url, save_path)
            tile_paths[(x, y)] = save_path

    return tile_paths, (x_start, y_start, x_end, y_end)

def latlon_to_pixel(lat, lon, zoom):
    """计算经纬度在整个瓦片坐标系中的像素位置"""
    n = 2.0 ** zoom
    x = (lon + 180.0) / 360.0 * n * TILE_SIZE
    y = (1.0 - math.log(math.tan(math.radians(lat)) + 1 / math.cos(math.radians(lat))) / math.pi) / 2.0 * n * TILE_SIZE
    return x, y

def crop_and_merge_tiles(tile_paths, top_left, bottom_right, zoom, output_image):
    """裁剪并合并瓦片"""
    x_start, y_start = deg2num(top_left[0], top_left[1], zoom)
    x_end, y_end = deg2num(bottom_right[0], bottom_right[1], zoom)

    # 计算左上角和右下角的像素坐标
    x_top_left, y_top_left = latlon_to_pixel(top_left[0], top_left[1], zoom)
    x_bottom_right, y_bottom_right = latlon_to_pixel(bottom_right[0], bottom_right[1], zoom)

    # 计算裁剪偏移量
    px_start_x = int(x_top_left % TILE_SIZE)
    px_start_y = int(y_top_left % TILE_SIZE)
    px_end_x = int(x_bottom_right % TILE_SIZE)
    px_end_y = int(y_bottom_right % TILE_SIZE)

    # 计算最终拼接图片的大小
    img_width = (x_end - x_start + 1) * TILE_SIZE
    img_height = (y_end - y_start + 1) * TILE_SIZE

    # 创建空白大图
    merged_image = Image.new('RGB', (img_width, img_height))

    # 逐个填充瓦片
    for (x, y), path in tile_paths.items():
        tile = Image.open(path)
        x_offset = (x - x_start) * TILE_SIZE
        y_offset = (y - y_start) * TILE_SIZE
        merged_image.paste(tile, (x_offset, y_offset))

    # 裁剪到指定区域
    cropped_image = merged_image.crop((px_start_x, px_start_y, img_width - (TILE_SIZE - px_end_x), img_height - (TILE_SIZE - px_end_y)))

    # **提高分辨率**
    new_size = (cropped_image.width * scale_factor, cropped_image.height * scale_factor)
    high_res_image = cropped_image.resize(new_size, Image.LANCZOS)  # 使用 LANCZOS 滤波，提高质量
    
    high_res_image.save(output_image)
    print(f"Saved high-resolution image to {output_image}")

# 示例参数
top_left = (8.9887, -79.5228)  # 左上角经纬度
bottom_right = (8.9852, -79.5183)  # 右下角经纬度
zoom = 15  # **提高 zoom 级别，增加清晰度**
base_url = "http://localhost/osm"
output_dir = "/home/muzi/Documents/tiles"
output_image = "/home/muzi/Documents/high_res_map.png"

# 下载瓦片
tile_paths, bounds = download_tiles_in_range(top_left, bottom_right, zoom, base_url, output_dir)

# 拼接并裁剪
crop_and_merge_tiles(tile_paths, top_left, bottom_right, zoom, output_image)
