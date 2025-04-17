import os
import json
import time
from typing import Dict, Any, List, Optional
from dataclasses import dataclass, asdict, field
@dataclass
class WALRecord:
    """使用dataclass简化日志记录类的定义"""
    lsn: int
    trans_id: str
    operation: str
    table: str
    before: Dict[str, Any]
    after: Dict[str, Any]
    timestamp: float = field(default_factory=time.time)

    def to_dict(self) -> Dict[str, Any]:
        return asdict(self)

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'WALRecord':
        return cls(**data)

class WAL:
    """WAL管理器"""
    def __init__(self, log_path: str):
        """初始化WAL管理器
        
        Args:
            log_path: 日志文件路径
        """
        self.log_path = log_path
        self.current_lsn = 0
        self.checkpoint_size = 200  # 修改为较小的值以便测试
        # 将archive_dir和checkpoint_dir放在log_path同级目录下
        base_dir = os.path.dirname(log_path)
        self.archive_dir = os.path.join(base_dir, "archive")
        self.checkpoint_dir = os.path.join(base_dir, "checkpoint")
        self.records_count = 0
        
        # 确保目录存在
        self._init_directories()
        
        # 创建文件（如果不存在）
        if not os.path.exists(self.log_path):
            with open(self.log_path, 'w') as f:
                pass
        
        self._init_records_count()
    
    def _write_to_log(self, data: str):
        """同步写入日志文件
        
        Args:
            data: 要写入的数据
        """
        with open(self.log_path, 'a') as f:
            f.write(data + '\n')
            f.flush()
            os.fsync(f.fileno())
    
    def append(self, trans_id: str, operation: str, 
              table: str, before: Dict[str, Any], after: Dict[str, Any]) -> int:
        """追加新的日志记录"""
        self.current_lsn += 1
        record = WALRecord(self.current_lsn, trans_id, operation, 
                            table, before, after)
        
        # 同步写入
        self._write_to_log(json.dumps(record.to_dict()))
        
        # 更新计数并检查是否需要创建检查点
        self.records_count += 1
        if self.records_count >= self.checkpoint_size:
            self._create_checkpoint_and_archive()
        
        return self.current_lsn
    
    def flush(self):
        """刷新日志到磁盘（现在是空操作，因为每次写入都是同步的）"""
        pass

    def recover(self) -> List[WALRecord]:
        """从日志文件中恢复记录"""
        try:
            # 首先检查最新的检查点
            checkpoint = self._find_latest_checkpoint()
            start_lsn = checkpoint["lsn"] if checkpoint else 0
            
            # 获取归档的状态
            final_state = {}
            archive_files = sorted([f for f in os.listdir(self.archive_dir) 
                                 if f.startswith("state_") and f.endswith(".json")])
            if archive_files:
                latest_archive = archive_files[-1]
                with open(os.path.join(self.archive_dir, latest_archive)) as f:
                    archive_data = json.load(f)
                    final_state = archive_data["state"]
            
            # 从检查点开始读取增量记录
            with open(self.log_path, 'r') as f:
                for line in f:
                    if line.strip():
                        record = WALRecord.from_dict(json.loads(line))
                        if record.lsn > start_lsn:
                            # 更新状态
                            if record.operation == "UPDATE":
                                if record.table not in final_state:
                                    final_state[record.table] = {}
                                final_state[record.table].update(record.after)
            
            # 将最终状态转换为WAL记录
            result_records = []
            for table, table_data in final_state.items():
                for key, value in table_data.items():
                    record = WALRecord(
                        lsn=self.current_lsn + 1,
                        trans_id="RECOVERY",
                        operation="UPDATE",
                        table=table,
                        before={},
                        after={key: value}
                    )
                    result_records.append(record)
            
            return result_records
        
        except Exception as e:
            print("Error during recovery: {}".format(e))
            return []

    def _create_checkpoint_and_archive(self):
        """异步创建检查点和归档"""
        try:
            checkpoint = self.create_checkpoint()
            self.archive_logs(checkpoint["lsn"])
            self.records_count = 0
        except Exception as e:
            print(f"Error during checkpoint/archive: {e}")
    
    def create_checkpoint(self) -> Dict[str, Any]:
        """创建检查点，包含更多信息"""
        checkpoint = {
            "timestamp": time.time(),
            "lsn": self.current_lsn,
            "records_count": self.records_count,
            "pending_writes": 0
        }
        
        # 在checkpoint_dir下创建checkpoint文件
        checkpoint_path = os.path.join(self.checkpoint_dir, 
                                     "checkpoint_{}.json".format(checkpoint['lsn']))
        
        with open(checkpoint_path, 'w') as f:
            json.dump(checkpoint, f)
        
        return checkpoint
    
    def _find_latest_checkpoint(self) -> Optional[Dict[str, Any]]:
        """查找最新的检查点"""
        # 在checkpoint_dir下查找checkpoint文件
        checkpoint_files = [f for f in os.listdir(self.checkpoint_dir) 
                          if f.startswith("checkpoint_")]
        
        if not checkpoint_files:
            return None
        
        latest_checkpoint = max(checkpoint_files)
        with open(os.path.join(self.checkpoint_dir, latest_checkpoint)) as f:
            return json.load(f)
    
    def __del__(self):
        """清理资源（不再需要停止线程）"""
        pass

    def recover_with_archives(self) -> List[WALRecord]:
        """从归档状态和当前日志中恢复所有记录
    
        恢复流程：
        1. 加载最新的归档状态
        2. 读取当前WAL文件中的记录
        3. 按LSN顺序应用所有更新
        
        Returns:
            转换后的WAL记录列表，表示最终状态
        """
        try:
            # 1. 获取最新归档状态
            final_state = {}  # {table: {key: value}}
            last_archive_lsn = 0
            
            archive_files = sorted([f for f in os.listdir(self.archive_dir) 
                                 if f.startswith("state_") and f.endswith(".json")])
            if archive_files:
                latest_archive = archive_files[-1]
                with open(os.path.join(self.archive_dir, latest_archive)) as f:
                    archive_data = json.load(f)
                    final_state = archive_data["state"]
                    last_archive_lsn = archive_data["metadata"]["checkpoint_lsn"]
            
            # 2. 读取并应用当前WAL文件中的所有记录
            if os.path.exists(self.log_path):
                with open(self.log_path, 'r') as f:
                    for line in f:
                        if line.strip():
                            record = WALRecord.from_dict(json.loads(line))
                            if record.operation == "UPDATE":
                                if record.table not in final_state:
                                    final_state[record.table] = {}
                                final_state[record.table].update(record.after)
            
            # 3. 将最终状态转换为WAL记录
            result_records = []
            for table, table_data in final_state.items():
                for key, value in table_data.items():
                    record = WALRecord(
                        lsn=self.current_lsn + 1,
                        trans_id="RECOVERY",
                        operation="UPDATE",
                        table=table,
                        before={},
                        after={key: value}
                    )
                    result_records.append(record)
            
            return result_records
        
        except Exception as e:
            print("Error during recovery with archives: {}".format(e))
            return []

    def _get_relevant_archives(self, checkpoint_lsn: int) -> List[str]:
        """获取与恢复相关的归档文件列表
    
        Args:
            checkpoint_lsn: 检查点的LSN
        
        Returns:
            相关归档文件的完整路径列表
        """
        if not os.path.exists(self.archive_dir):
            return []
        
        archive_files = []
        for filename in os.listdir(self.archive_dir):
            if not filename.startswith("wal_") or not filename.endswith(".log"):
                continue
            
            # 从文件名解析LSN范围
            try:
                # 文件名格式：wal_START-LSN_END-LSN.log
                _, start_lsn, end_lsn = filename.replace(".log", "").split("_")
                end_lsn = int(end_lsn)
                
                # 只选择包含检查点LSN之后记录的文件
                if end_lsn >= checkpoint_lsn:
                    archive_files.append(os.path.join(self.archive_dir, filename))
            except ValueError:
                continue
        
        # 按文件名排序，确保按时间顺序处理
        return sorted(archive_files)

    def _read_log_file(self, file_path: str) -> List[WALRecord]:
        """读取日志文件中的所有记录
    
        Args:
            file_path: 日志文件路径
        
        Returns:
            文件中的WALRecord列表
        """
        records = []
        if not os.path.exists(file_path):
            return records
        
        with open(file_path, 'r') as f:
            for line in f:
                if line.strip():
                    try:
                        data = json.loads(line)
                        record = WALRecord.from_dict(data)
                        records.append(record)
                    except json.JSONDecodeError:
                        print(f"Error decoding log record: {line}")
                    except Exception as e:
                        print(f"Error processing log record: {e}")
        
        return records

    def _sort_and_deduplicate_records(self, records: List[WALRecord]) -> List[WALRecord]:
        """对记录进行排序和去重
    
        Args:
            records: 原始记录列表
        
        Returns:
            排序和去重后的记录列表
        """
        # 按LSN排序
        sorted_records = sorted(records, key=lambda r: r.lsn)
        
        # 使用字典去重，保留最新的记录
        unique_records = {}
        for record in sorted_records:
            unique_records[record.lsn] = record
        
        return list(unique_records.values())

    def _init_records_count(self):
        """初始化记录计数
    
        包括当前WAL文件和所有归档文件中的记录
        """
        self.records_count = 0
        
        # 计算归档文件中的记录数
        if os.path.exists(self.archive_dir):
            for filename in os.listdir(self.archive_dir):
                if filename.startswith("wal_") and filename.endswith(".log"):
                    file_path = os.path.join(self.archive_dir, filename)
                    with open(file_path, 'r') as f:
                        self.records_count += sum(1 for line in f if line.strip())
        
        # 计算当前WAL文件中的记录数
        if os.path.exists(self.log_path):
            with open(self.log_path, 'r') as f:
                self.records_count += sum(1 for line in f if line.strip())

    def archive_logs(self, checkpoint_lsn: int):
        """归档检查点之前的日志记录，将其转换为最终数据状态
    
        Args:
            checkpoint_lsn: 检查点的LSN
        """
        try:
            # 读取当前WAL文件中需要归档的记录
            records_to_archive = []
            with open(self.log_path, 'r') as f:
                for line in f:
                    if line.strip():
                        record = WALRecord.from_dict(json.loads(line))
                        if record.lsn <= checkpoint_lsn:
                            records_to_archive.append(record)
            
            if not records_to_archive:
                return
            
            # 将WAL记录转换为最终数据状态
            final_state = {}  # 格式: {table: {key: value}}
            for record in records_to_archive:
                if record.operation == "UPDATE":
                    if record.table not in final_state:
                        final_state[record.table] = {}
                    # 遍历after字典中的所有键值对
                    for key, value in record.after.items():
                        final_state[record.table][key] = value
            
            # 创建归档文件（使用JSON格式存储最终状态）
            timestamp = int(time.time())
            archive_filename = "state_{}.json".format(timestamp)
            archive_path = os.path.join(self.archive_dir, archive_filename)
            
            # 保存最终状态和元数据
            archive_data = {
                "metadata": {
                    "checkpoint_lsn": checkpoint_lsn,
                    "timestamp": timestamp,
                    "record_count": len(records_to_archive)
                },
                "state": final_state
            }
            
            # 写入归档文件
            with open(archive_path, 'w') as f:
                json.dump(archive_data, f, indent=2)
            
            # 清理原WAL文件中已归档的记录
            remaining_records = []
            with open(self.log_path, 'r') as f:
                for line in f:
                    if line.strip():
                        record = WALRecord.from_dict(json.loads(line))
                        if record.lsn > checkpoint_lsn:
                            remaining_records.append(line)
            
            # 重写WAL文件，只保留未归档的记录
            with open(self.log_path, 'w') as f:
                for line in remaining_records:
                    f.write(line)
        
        except Exception as e:
            print("Error during log archiving: {}".format(e))

    def _init_directories(self):
        """初始化所需的目录"""
        # 确保WAL文件所在目录存在
        os.makedirs(os.path.dirname(self.log_path), exist_ok=True)
        # 确保归档目录存在
        os.makedirs(self.archive_dir, exist_ok=True)
        # 确保checkpoint目录存在
        os.makedirs(self.checkpoint_dir, exist_ok=True)



