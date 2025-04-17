import unittest
import os
import json
from database import Database

class TestWAL(unittest.TestCase):
    """WAL机制的单元测试类
    
    测试内容：
     1. 基本的数据更新操作
     2. 系统崩溃后的数据恢复
     3. 归档功能
    """
    def __init__(self, *args, **kwargs):
        """初始化测试类，添加清理标志"""
        super().__init__(*args, **kwargs)
        # 默认不清理文件，方便观察
        self.should_cleanup = False

    def setUp(self):
        """测试前的准备工作
        
        1. 设置测试用的目录结构
        2. 确保测试开始前相关文件不存在
        3. 创建新的数据库实例
        """
        # 使用当前目录作为基础目录
        self.test_dir = os.getcwd()
        
        # 创建WAL日志目录
        self.wal_dir = os.path.join(self.test_dir, "wal_logs")
        if not os.path.exists(self.wal_dir):
            os.makedirs(self.wal_dir)
        self.wal_path = os.path.join(self.wal_dir, "test_wal.log")
        
        # 清理之前的测试文件
        if os.path.exists(self.wal_path):
            os.remove(self.wal_path)
        
        # 清理之前的归档目录
        archive_dir = os.path.join(self.test_dir, "archive")
        if os.path.exists(archive_dir):
            for f in os.listdir(archive_dir):
                os.remove(os.path.join(archive_dir, f))
            os.rmdir(archive_dir)
        
        # 创建数据库实例时传入checkpoint目录
        self.db = Database(self.wal_path)
    
    def tearDown(self):
        """测试后的清理工作
        
        只有在should_cleanup为True时才清理文件
        """
        if not self.should_cleanup:
            return
        
        try:
            # 清理WAL日志文件和目录
            if os.path.exists(self.wal_path):
                os.remove(self.wal_path)
            if os.path.exists(self.wal_dir):
                os.rmdir(self.wal_dir)
            
            # 清理checkpoint文件和目录
            if os.path.exists(self.checkpoint_dir):
                for f in os.listdir(self.checkpoint_dir):
                    os.remove(os.path.join(self.checkpoint_dir, f))
                os.rmdir(self.checkpoint_dir)
            
            # 清理归档目录
            archive_dir = os.path.join(self.test_dir, "archive")
            if os.path.exists(archive_dir):
                for f in os.listdir(archive_dir):
                    os.remove(os.path.join(archive_dir, f))
                os.rmdir(archive_dir)
        except Exception as e:
            print("Error during cleanup: {}".format(e))
    
    def test_basic_operations(self):
        """测试基本的更新操作，使用更多数据
        
        验证：
        1. 数据更新是否成功
        2. 更新后的值是否正确
        3. 多表操作
        4. 多种数据类型
        """
        # 账户表操作
        accounts_data = {
            "Alice": 1000,
            "Bob": 500,
            "Charlie": 2000,
            "David": 1500,
            "Eve": 3000
        }
        
        # 用户信息表操作
        user_info_data = {
            "Alice": {"age": 25, "city": "New York", "active": True},
            "Bob": {"age": 30, "city": "London", "active": False},
            "Charlie": {"age": 35, "city": "Paris", "active": True},
            "David": {"age": 28, "city": "Tokyo", "active": True},
            "Eve": {"age": 32, "city": "Berlin", "active": True}
        }
        
        # 商品表操作
        products_data = {
            "prod1": {"name": "Laptop", "price": 1200, "stock": 50},
            "prod2": {"name": "Phone", "price": 800, "stock": 100},
            "prod3": {"name": "Tablet", "price": 600, "stock": 75},
            "prod4": {"name": "Watch", "price": 300, "stock": 150},
            "prod5": {"name": "Headphones", "price": 200, "stock": 200}
        }
        
        # 更新账户表
        for user, balance in accounts_data.items():
            self.db.update("T1", "accounts", user, balance)
        
        # 更新用户信息表
        for user, info in user_info_data.items():
            self.db.update("T1", "user_info", user, info)
        
        # 更新商品表
        for prod_id, info in products_data.items():
            self.db.update("T1", "products", prod_id, info)
        
        self.db.commit()
        
        # 验证账户表数据
        for user, balance in accounts_data.items():
            self.assertEqual(self.db.get("accounts", user), balance)
        
        # 验证用户信息表数据
        for user, info in user_info_data.items():
            self.assertEqual(self.db.get("user_info", user), info)
        
        # 验证商品表数据
        for prod_id, info in products_data.items():
            self.assertEqual(self.db.get("products", prod_id), info)
    
    def test_recovery(self):
        """测试数据库恢复功能
        
        验证：
        1. 写入数据后系统崩溃
        2. 重启后是否能正确恢复数据
        """
        self.db.update("T1", "accounts", "Alice", 1000)
        self.db.update("T1", "accounts", "Bob", 500)
        self.db.commit()
        
        # 创建新的数据库实例，模拟重启
        new_db = Database(self.wal_path)
        
        # 验证数据是否正确恢复
        self.assertEqual(new_db.get("accounts", "Alice"), 1000)
        self.assertEqual(new_db.get("accounts", "Bob"), 500)
    
    def test_archiving(self):
        """测试归档功能，使用更多数据触发归档
        
        验证：
        1. 写入大量数据触发归档
        2. 检查归档文件是否正确生成
        3. 验证归档的数据是否正确
        """
        print("\nStarting archiving test...")
        print("Test directory:", self.test_dir)
        
        # 生成大量测试数据
        for i in range(100):  # 增加到100个用户
            # 账户数据
            self.db.update("T1", "accounts", f"user_{i}", 1000 + i * 100)
            
            # 用户信息
            user_info = {
                "age": 20 + i % 40,
                "city": ["New York", "London", "Paris", "Tokyo", "Berlin"][i % 5],
                "active": i % 3 == 0,
                "score": i * 10,
                "registration_date": f"2024-{(i % 12) + 1:02d}-{(i % 28) + 1:02d}"
            }
            self.db.update("T1", "user_info", f"user_{i}", user_info)
            
            # 交易记录
            transaction = {
                "amount": 100 + i * 50,
                "type": ["deposit", "withdrawal", "transfer"][i % 3],
                "timestamp": f"2024-03-{i % 30 + 1:02d} {i % 24:02d}:{i % 60:02d}:00"
            }
            self.db.update("T1", "transactions", f"tx_{i}", transaction)
        
        self.db.commit()
        print("Large dataset committed")
        
        # 检查归档目录
        archive_dir = os.path.join(self.test_dir, "wal_logs", "archive")
        print("Looking for archives in:", archive_dir)
        
        # 确保目录存在
        self.assertTrue(os.path.exists(archive_dir), "Archive directory does not exist")
        
        archive_files = [f for f in os.listdir(archive_dir) 
                        if f.startswith("state_") and f.endswith(".json")]
        print("Found archive files:", archive_files)
        
        # 验证是否生成了归档文件
        self.assertTrue(len(archive_files) > 0, "No archive file was created")
        
        # 读取最新的归档文件
        latest_archive = max(archive_files)
        archive_path = os.path.join(archive_dir, latest_archive)
        print("Reading archive file:", archive_path)
        
        with open(archive_path) as f:
            archive_data = json.load(f)
            print("Archive content sample:", json.dumps(
                {k: list(v.keys())[:5] for k, v in archive_data["state"].items()}, 
                indent=2
            ))
        
        # 验证归档的数据是否正确（抽样检查）
        self.assertIn("accounts", archive_data["state"])
        self.assertIn("user_info", archive_data["state"])
        self.assertIn("transactions", archive_data["state"])
        
        # 检查几个随机样本
        for i in [0, 42, 66]:
            user_key = f"user_{i}"
            self.assertEqual(
                archive_data["state"]["accounts"][user_key], 
                1000 + i * 100
            )
            self.assertEqual(
                archive_data["state"]["user_info"][user_key]["age"], 
                20 + i % 40
            )

if __name__ == '__main__':
    unittest.main()
