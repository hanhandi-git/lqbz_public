# clip_feature_extraction.py
import torch
from transformers import CLIPProcessor, CLIPModel
from PIL import Image

def extract_clip_features(image_path, text):
    """
    使用 CLIP 提取图像和文本的嵌入特征
    :param image_path: String, 图像文件路径
    :param text: String, 文本输入
    :return: 图像嵌入和文本嵌入
    """
    model = CLIPModel.from_pretrained("openai/clip-vit-base-patch32")
    processor = CLIPProcessor.from_pretrained("openai/clip-vit-base-patch32")

    image = Image.open(image_path).convert("RGB")
    inputs = processor(text=[text], images=image, return_tensors="pt", padding=True)
    outputs = model(**inputs)

    image_features = outputs.image_embeds
    text_features = outputs.text_embeds
    return image_features, text_features

if __name__ == "__main__":
    image_path = "example.jpg"  # 替换为实际路径
    text = "This is an example description."
    image_features, text_features = extract_clip_features(image_path, text)
    print("Image Features:", image_features)
    print("Text Features:", text_features)
