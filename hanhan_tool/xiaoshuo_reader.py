import requests
import time
from typing import List, Optional
from bs4 import BeautifulSoup
import re
import os

def fetch_webpage(url: str, max_retries: int = 3, delay: float = 0.2) -> Optional[str]:
    """
    获取网页内容,带重试和延时机制
    
    Args:
        url: 要获取的URL
        max_retries: 最大重试次数
        delay: 请求间隔时间(秒)
    
    Returns:
        网页内容,失败返回None
    """
    headers = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36'
    }
    
    for attempt in range(max_retries):
        try:
            time.sleep(delay)  # 添加延时避免请求过快
            response = requests.get(url, headers=headers, timeout=10)
            response.raise_for_status()
            return response.text
        except requests.RequestException as e:
            print("请求错误 (尝试 {}/{}): {}".format(attempt + 1, max_retries, e))
            if attempt == max_retries - 1:
                return None
            time.sleep(delay * (attempt + 1))  # 重试时增加延时

def save_content(content: str, filename: str) -> bool:
    """
    保存内容到文件
    
    Args:
        content: 要保存的内容
        filename: 文件名
    
    Returns:
        是否保存成功
    """
    try:
        with open(filename, 'w', encoding='utf-8') as file:
            file.write(content)
        return True
    except IOError as e:
        print("保存文件失败: {}".format(e))
        return False

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

def generate_chapter_urls(base_url: str, start_chapter: int, page: int):
    """
    生成章节URL的生成器函数
    
    Args:
        base_url: 基础URL,如 https://www.xiaoshubao.net/read/462167/
        start_chapter: 章节号
        page: 页码
    
    Yields:
        按需生成的章节URL
    """
    page_url = "{}{}_{}.html".format(base_url, start_chapter, page)
    return page_url

def parse_chapter_to_txt(html_content, output_file=None):
    """
    从HTML内容中解析小说章节内容
    
    Args:
        html_content: HTML文本内容
        output_file: 输出的txt文件路径(可选)
    
    Returns:
        解析出的文本内容,如果解析失败返回空字符串
    """
    try:
        # 使用BeautifulSoup解析HTML
        soup = BeautifulSoup(html_content, 'html.parser')
        
        # 获取content div
        content_div = soup.find('div', id='content')
        if not content_div:
            return ""
            
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
        
        # 如果提供了输出文件路径则写入文件
        if output_file and text:
            with open(output_file, 'w', encoding='utf-8') as f:
                f.write(text)
                
        return text
        
    except Exception as e:
        print("解析出错: {}".format(e))
        return ""

def archive_chapter(chapter_num: int, base_dir: str) -> bool:
    """
    将同一章节的分页文件归档为一个完整文件
    
    Args:
        chapter_num: 章节号
        base_dir: 基础目录路径
    
    Returns:
        是否归档成功
    """
    try:
        # 构建临时目录和归档目录路径
        temp_dir = os.path.join(base_dir, 'temp')
        archive_dir = os.path.join(base_dir, 'archive')
        
        # 确保归档目录存在
        os.makedirs(archive_dir, exist_ok=True)
        
        # 获取该章节的所有分页文件
        chapter_files = []
        base_file = os.path.join(temp_dir, f'chapter_{chapter_num}.txt')
        if os.path.exists(base_file):
            chapter_files.append(base_file)
            
        page = 1
        while True:
            page_file = os.path.join(temp_dir, f'chapter_{chapter_num}_{page}.txt')
            if not os.path.exists(page_file):
                break
            chapter_files.append(page_file)
            page += 1
            
        if not chapter_files:
            return False
            
        # 合并所有分页内容
        combined_content = []
        for file_path in chapter_files:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read().strip()
                if content:
                    combined_content.append(content)
                    
        # 保存归档文件
        archive_file = os.path.join(archive_dir, f'chapter_{chapter_num}.txt')
        with open(archive_file, 'w', encoding='utf-8') as f:
            f.write('\n\n'.join(combined_content))
            
        # 删除原始分页文件
        for file_path in chapter_files:
            os.remove(file_path)
            
        return True
        
    except Exception as e:
        print(f"归档章节 {chapter_num} 失败: {e}")
        return False

def main():
    """主函数"""
    base_url = "https://www.xiaoshubao.net/read/462167/"
    start_chapter = 1281
    
    # 创建基础目录
    base_dir = os.path.join(os.getcwd(), 'xiaoshuo')
    temp_dir = os.path.join(base_dir, 'temp')
    os.makedirs(temp_dir, exist_ok=True)
    
    current_chapter = start_chapter
    page = 0
    
    while True:
        try:
            url = generate_chapter_urls(base_url, current_chapter, page)    
            print("正在获取: {}".format(url))
            
            content = fetch_webpage(url)
            if not content:
                print("获取失败: {}".format(url))
                if page == 0:  # 如果主章节页面获取失败,继续下一章
                    current_chapter += 1
                    page = 0
                    continue
                else:  # 如果分页获取失败,转到下一章
                    # 尝试归档当前章节
                    archive_chapter(current_chapter, base_dir)
                    current_chapter += 1 
                    page = 0
                    continue
                    
            # 解析内容
            text = parse_chapter_to_txt(content)
            if len(text) > 0:  # 有内容则保存
                filename = "chapter_{}"
                if page > 0:
                    filename = filename + "_{}"
                filename = filename + ".txt"
                
                if page > 0:
                    filename = filename.format(current_chapter, page)
                else:
                    filename = filename.format(current_chapter)
                
                # 使用临时目录保存分页文件
                filepath = os.path.join(temp_dir, filename)
                with open(filepath, 'w', encoding='utf-8') as f:
                    f.write(text)
                print("已保存: {}".format(filename))
                
                if page == 0:  # 主章节有内容,继续获取分页
                    page += 1
                else:  # 分页有内容,继续获取下一分页
                    page += 1
            else:  # 没有内容
                print("没有内容: {}".format(url))
                # 尝试归档当前章节
                archive_chapter(current_chapter, base_dir)
                current_chapter += 1
                page = 0
                    
        except StopIteration:
            break
            
    # 程序结束前归档最后一章
    archive_chapter(current_chapter, base_dir)

if __name__ == "__main__":
    main()