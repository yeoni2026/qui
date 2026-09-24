#!/bin/bash
set -e

# 1. 깃허브 정보 및 저장 경로 설정
GITHUB_USER="yeoni2026"       # 본인 GitHub 아이디로 변경
REPO="qui"
INSTALL_DIR="/usr/local/bin"
BINARY_NAME="qui-bin"         # 실제 실행 바이너리 이름

# 2. OS 및 아키텍처(CPU) 판별
OS=$(uname -s)
ARCH=$(uname -m)

if [ "$OS" = "Linux" ]; then
    FILE="qui-linux-amd64"
elif [ "$OS" = "Darwin" ]; then
    FILE="qui-darwin-arm64"
else
    echo "지원하지 않는 OS입니다: $OS"
    exit 1
fi

# 3. 최신 릴리즈 바이너리 다운로드
echo "==> 최신 버전 다운로드 중 ($FILE)..."
URL="https://github.com/$GITHUB_USER/$REPO/releases/latest/download/$FILE"
curl -fsSL "$URL" -o /tmp/qui-bin

# 4. /usr/local/bin으로 이동 및 실행 권한 부여
echo "==> 바이너리 설치 중..."
chmod +x /tmp/qui-bin
# 쓰기 권한이 필요할 수 있으므로 sudo 처리
if [ -w "$INSTALL_DIR" ]; then
    mv /tmp/qui-bin "$INSTALL_DIR/$BINARY_NAME"
else
    sudo mv /tmp/qui-bin "$INSTALL_DIR/$BINARY_NAME"
fi

# 5. ~/.bashrc (또는 ~/.zshrc)에 쉘 함수 등록
RC_FILE="$HOME/.bashrc"
# macOS 기본 쉘이 zsh인 경우 대응
if [ "$SHELL" = "*/zsh" ] || [ -f "$HOME/.zshrc" ]; then
    RC_FILE="$HOME/.zshrc"
fi

# 중복 등록 방지 검사 후 추가
if ! grep -q "qui()" "$RC_FILE" 2>/dev/null; then
    echo "==> $RC_FILE 에 qui 함수 등록 중..."
    echo '
# --- qui shell integration ---
qui() {
    local cmd
    # 1. 바이너리 실행 후 stdout은 cmd에 담고, stderr는 화면에 그대로 통과
    cmd=$(qui-bin "$@")
    local status=$?

    # 2. 만약 qui-bin이 비정상 종료(exit != 0)했다면 eval을 하지 않고 그 상태 코드 그대로 반환
    if [ $status -ne 0 ]; then
        return $status
    fi

    # 3. 정상 종료(0)되었고, 실행할 명령어가 stdout으로 들어온 경우에만 eval 실행
    if [ -n "$cmd" ]; then
        eval "$cmd"
    fi
}
' >> "$RC_FILE"
fi

echo "==> 설치가 완료되었습니다!"
echo "새 터미널을 열거나 'source $RC_FILE'을 실행해 바로 사용하세요."