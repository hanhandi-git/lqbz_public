from typing import Dict, Any
from wal import WAL
import os

class Database:
    """简单的内存数据库实现，支持WAL机制
    
    主要功能：
    1. 基本的键值对存储
    2. 使用WAL确保数据持久性
    3. 系统重启时的数据恢复
    """
    def __init__(self, wal_path: str, checkpoint_dir=None):
        """初始化数据库
        
        Args:
            wal_path: WAL日志文件路径
            checkpoint_dir: checkpoint目录路径（可选）
        """
        self.wal_path = wal_path
        self.checkpoint_dir = checkpoint_dir or os.path.dirname(wal_path)
        self.data: Dict[str, Dict[str, Any]] = {}  # 内存中的数据存储
        self.wal = WAL(wal_path)  # WAL管理器
        self.recover_from_wal()  # 启动时从WAL恢复数据
    
    def recover_from_wal(self):
        """从WAL（包括归档）恢复数据库状态"""
        # 使用recover_with_archives替代recover
        records = self.wal.recover_with_archives()
        for record in records:
            if record.operation == "UPDATE":
                if record.table not in self.data:
                    self.data[record.table] = {}
                self.data[record.table].update(record.after)
    
    def update(self, trans_id: str, table: str, key: str, value: Any):
        """更新数据库中的值
        
        遵循WAL原则：先写日志，再更新数据
        
        Args:
            trans_id: 事务ID
            table: 表名
            key: 键
            value: 值
        """
        if table not in self.data:
            self.data[table] = {}
        
        # 记录修改前的值
        before = {key: self.data[table].get(key)}
        after = {key: value}
        
        # 先写日志
        self.wal.append(trans_id, "UPDATE", table, before, after)
        
        # 再更新数据
        self.data[table][key] = value
    
    def get(self, table: str, key: str) -> Any:
        """获取数据库中的值
        
        Args:
            table: 表名
            key: 键
            
        Returns:
            对应的值，如果不存在则返回None
        """
        return self.data.get(table, {}).get(key)
    
    def commit(self):
        """提交事务，确保日志写入磁盘"""
        self.wal.flush()
