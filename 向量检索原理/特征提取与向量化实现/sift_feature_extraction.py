# sift_feature_extraction.py
import cv2

def extract_sift_features(image_path):
    """
    使用 SIFT 提取图像特征
    :param image_path: String, 图像文件路径
    :return: 关键点和描述符
    """
    img = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)
    sift = cv2.SIFT_create()
    keypoints, descriptors = sift.detectAndCompute(img, None)
    return keypoints, descriptors

if __name__ == "__main__":
    image_path = "example.jpg"  # 替换为实际路径
    keypoints, descriptors = extract_sift_features(image_path)
    print(f"Number of Keypoints: {len(keypoints)}")
    print("Descriptors Shape:", descriptors.shape)
