from bs4 import BeautifulSoup
import re

def parse_chapter_to_txt(html_content, output_file):
    """
    从HTML内容中解析小说章节内容并保存到txt文件
    
    Args:
        html_content: HTML文本内容
        output_file: 输出的txt文件路径
    """
    # 使用BeautifulSoup解析HTML
    soup = BeautifulSoup(html_content, 'html.parser')
    
    # 获取content div
    content_div = soup.find('div', id='content')
    if not content_div:
        raise ValueError("Cannot find content div")
        
    # 删除提示段落
    for p in content_div.find_all('p'):
        p.decompose()
        
    # 获取所有文本节点并按<br>分割
    paragraphs = []
    for text in content_div.stripped_strings:
        # 去除多余空白字符
        text = re.sub(r'\s+', ' ', text.strip())
        if text:
            paragraphs.append(text)
            
    # 使用两个换行符拼接段落
    text = '\n\n'.join(paragraphs)
    
    # 写入文件
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(text)


# 读取HTML文件
with open('chapter_1.html', 'r', encoding='utf-8') as f:
    html_content = f.read()
    
# 解析并保存到txt
parse_chapter_to_txt(html_content, 'chapter_1.txt')