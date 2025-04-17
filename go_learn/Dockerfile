FROM golang:1.21

# 直接添加缺少的GPG密钥
RUN apt-get update -y || true && \
    apt-get install -y --no-install-recommends gnupg && \
    apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 0E98404D386FA1D9 && \
    apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 6ED0E7B82643E131 && \
    apt-key adv --keyserver keyserver.ubuntu.com --recv-keys F8D2585B8783D481 && \
    apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 54404762BBB6E853 && \
    apt-key adv --keyserver keyserver.ubuntu.com --recv-keys BDE6D2B9216EC7A8

# 安装必要的工具
RUN apt-get update && apt-get install -y \
    git \
    curl \
    bash \
    openssh-client \
    make \
    gcc \
    libc-dev

# 设置工作目录
WORKDIR /app

# 设置环境变量
ENV GO111MODULE=on \
    CGO_ENABLED=0 \
    GOOS=linux \
    GOARCH=amd64

# 安装一些常用的Go工具
RUN go install github.com/go-delve/delve/cmd/dlv@latest && \
    go install golang.org/x/tools/gopls@latest && \
    go install github.com/golangci/golangci-lint/cmd/golangci-lint@latest

# 创建一个非root用户
RUN useradd -m -s /bin/bash -u 1000 gouser
USER gouser
WORKDIR /home/gouser

# 为VS Code Server创建必要的目录
RUN mkdir -p /home/gouser/.vscode-server/bin && \
    mkdir -p /home/gouser/.vscode-server/extensions && \
    mkdir -p /home/gouser/.cursor-server/bin && \
    mkdir -p /home/gouser/.cursor-server/extensions

# 配置工作目录
WORKDIR /app

# 默认命令
CMD ["tail", "-f", "/dev/null"]