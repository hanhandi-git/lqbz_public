# bert_vectorization.py
from transformers import BertTokenizer, BertModel
import torch

def bert_vectorize(sentence):
    """
    使用 BERT 对句子生成嵌入向量
    :param sentence: String, 输入句子
    :return: 嵌入向量
    """
    tokenizer = BertTokenizer.from_pretrained("bert-base-uncased")
    model = BertModel.from_pretrained("bert-base-uncased")
    inputs = tokenizer(sentence, return_tensors="pt", truncation=True, max_length=512)
    outputs = model(**inputs)
    return outputs.last_hidden_state.mean(dim=1)

if __name__ == "__main__":
    sentence = "This is a sample sentence for BERT."
    vector = bert_vectorize(sentence)
    print("BERT Embedding Vector:\n", vector)
