FROM golang:1.22

WORKDIR /app

# 最小化的安装命令
RUN apt-get install -y --no-install-recommends vim nano git curl wget htop net-tools dnsutils iputils-ping telnet lsof procps zip unzip jq tree tmux && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# 安装指定兼容版本的Go开发工具
RUN go install golang.org/x/tools/gopls@v0.14.2 && \
    go install github.com/go-delve/delve/cmd/dlv@latest && \
    go install github.com/fatih/gomodifytags@latest && \
    go install github.com/cweill/gotests/gotests@latest && \
    go install github.com/josharian/impl@latest

# 设置中国大陆用户友好的GOPROXY
ENV GOPROXY=https://goproxy.cn,direct

# 启动时保持容器运行
CMD ["tail", "-f", "/dev/null"]