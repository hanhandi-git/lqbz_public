# mfcc_feature_extraction.py
import librosa

def extract_mfcc(audio_path):
    """
    提取音频的 MFCC 特征
    :param audio_path: String, 音频文件路径
    :return: MFCC 特征矩阵
    """
    y, sr = librosa.load(audio_path, sr=None)
    mfcc_features = librosa.feature.mfcc(y=y, sr=sr, n_mfcc=13)
    return mfcc_features

if __name__ == "__main__":
    audio_path = "example.wav"  # 替换为实际路径
    mfcc_features = extract_mfcc(audio_path)
    print("MFCC Features Shape:", mfcc_features.shape)
