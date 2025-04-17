# -*- coding: utf-8 -*-
# 在这个代码片段中，使用了 MapReduce 的概念来处理 Web 服务器的日志，以生成每个 URL 的点击次数。下面是对代码的详细解释：
# 通过使用 MapReduce 的 mapper 和 reducer 方法，这段代码能够有效地处理 Web 服务器的日志，统计每个 URL 在特定时间段内的访问次数。mapper 负责将日志行转换为键值对，而 reducer 则负责对相同键的值进行汇总。这种方法非常适合处理大规模数据集，因为它可以并行处理数据并减少计算负担。
 from mrjob.job import MRJob


class HitCounts(MRJob):

    def extract_url(self, line):
        """Extract the generated url from the log line."""
        pass

    def extract_year_month(self, line):
        """Return the year and month portions of the timestamp."""
        pass

    def mapper(self, _, line):
        """Parse each log line, extract and transform relevant lines.

        Emit key value pairs of the form:

        (2016-01, url0), 1
        (2016-01, url0), 1
        (2016-01, url1), 1
        """
        url = self.extract_url(line)
        period = self.extract_year_month(line)
        yield (period, url), 1

    def reducer(self, key, values):
        """Sum values for each key.

        (2016-01, url0), 2
        (2016-01, url1), 1
        """
        yield key, sum(values)

    def steps(self):
        """Run the map and reduce steps."""
        return [
            self.mr(mapper=self.mapper,
                    reducer=self.reducer)
        ]


if __name__ == '__main__':
    HitCounts.run()
