#!/usr/bin/env bash
set -e

# 1. 실행 파일 삭제
INSTALL_DIR="/usr/local/bin"
if [ -f "$INSTALL_DIR/qui-bin" ]; then
    rm -f "$INSTALL_DIR/qui-bin"
    echo "✔ $INSTALL_DIR/qui-bin 바이너리를 삭제했습니다."
fi

# 2. 쉘 설정 파일에서 블록 제거하는 함수
clean_rc() {
    local rc_file="$1"
    if [ -f "$rc_file" ]; then
        if grep -q "# >>> qui initialization >>>" "$rc_file"; then
            # 시작 주석부터 끝 주석까지 통째로 삭제
            sed -i.bak '/# >>> qui initialization >>>/,/# <<< qui initialization <<</d' "$rc_file"
            rm -f "${rc_file}.bak"
            echo "✔ $rc_file 에서 qui 설정을 제거했습니다."
        fi
    fi
}

# bash와 zsh 둘 다 검사해서 청소
clean_rc "$HOME/.bashrc"
clean_rc "$HOME/.zshrc"

echo "qui가 성공적으로 삭제되었습니다."
echo "현재 터미널에 반영하려면 source ~/.bashrc (또는 zshrc)를 실행하거나 터미널을 다시 열어주세요."