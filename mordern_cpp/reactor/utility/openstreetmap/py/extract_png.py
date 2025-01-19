import os

def extract_images(meta_file, output_dir):
    """
    从 .meta 文件中提取嵌套的 .png 图像并保存到指定目录。
    
    Args:
        meta_file (str): 输入的 .meta 文件路径。
        output_dir (str): 保存提取图像的输出目录。
    """
    # 确保输出目录存在
    os.makedirs(output_dir, exist_ok=True)
    
    # PNG 文件标志
    png_magic = b'\x89PNG\r\n\x1a\n'
    count = 0  # 记录提取的图片数量

    with open(meta_file, 'rb') as f:
        data = f.read()  # 读取整个文件内容

    pos = 0
    while pos < len(data):
        # 查找 PNG 文件的起始位置
        pos = data.find(png_magic, pos)
        if pos == -1:
            break

        # 查找 PNG 文件的结束位置
        end = data.find(b'IEND', pos) + 8  # PNG 文件以 'IEND' 结尾，外加 8 字节校验码
        if end > pos:
            # 提取 PNG 数据
            png_data = data[pos:end]
            
            # 保存为单独的 PNG 文件
            output_path = os.path.join(output_dir, f"tile_{count}.png")
            with open(output_path, 'wb') as img_file:
                img_file.write(png_data)
            
            print(f"提取图像：{output_path}")
            count += 1
            pos = end  # 更新位置，继续查找下一个 PNG
        else:
            break

    print(f"共提取 {count} 张图片，保存到 {output_dir}")

# 示例用法
meta_file_path = "/var/lib/mod_tile/default/1/0/0/0/0/0.meta"  # 替换为你的 .meta 文件路径
output_directory = "./extracted_tiles"  # 保存图片的目录
extract_images(meta_file_path, output_directory)
