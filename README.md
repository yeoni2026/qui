# qui

`qui`는 터미널 명령어 단축키 매니저입니다.  
자주 쓰는 명령어를 `qui 1`부터 `qui 9`까지 등록하고 빠르게 실행할 수 있습니다.

---

### 설치

```bash
curl -fsSL https://raw.githubusercontent.com/yeoni2026/qui/main/install.sh | bash
```

### 삭제

```bash
curl -fsSL https://raw.githubusercontent.com/yeoni2026/qui/main/uninstall.sh | bash
```

---

### 단축키 등록

- **지정 번호 등록**: `qui <num> <cmd>`

예시:
```bash
# qui 1에 명령어를 등록합니다.
qui 1 echo "hello world"

# qui 3에 명령어를 등록합니다.
qui 3 git add .

# 직전에 썼던 명령어를 qui 4에 등록하고 싶다면
qui 4 !!
```

- **자동 번호 등록**: `qui auto <cmd>`

예시:
```bash
# 현재 비어있는 가장 빠른 단축 번호에 명령어를 자동 등록합니다.
qui auto cd ~/projects/qui
>> Command assigned to slot 2. Run with 'qui 2'.
```

---

### 단축키 사용
`qui <num>`

예시:
```bash
# qui 1에 등록된 명령어를 실행합니다.
qui 1
>> echo "hello world"
>> hello world

# 등록된 명령어가 없을 경우
qui 5
>> Shortcut 5 is not set yet.
```

---

### 단축키 상태 & qui 사용법 확인
`qui`

예시:
```bash
qui
>> Usage:
>>   qui <num>          Execute shortcut
>>   qui <num> <cmd>    Register shortcut
>>
>> [qui shortcuts]
>>   1: echo "hello world"
>>   2: cd ~/projects/qui
>>   ...
>>   9: (empty)
```

---

### 단축키 초기화 
`qui clean`

명령어 사용시 전체 단축키가 초기화됩니다.

예시:
```bash
qui clean
qui
>>   ...
>> [qui shortcuts]
>>   1. (empty)
>>   2. (empty)
>>   ...
>>   9. (empty)
```