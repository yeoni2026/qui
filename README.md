# qui

`qui`는 터미널 명령어 단축키 매니저입니다.  
자주 쓰는 명령어를 `qui 0`부터 `qui 9`까지 등록하고 빠르게 실행할 수 있습니다.

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
`qui <num> <cmd>`

예시:
```bash
# qui 1에 명령어를 등록합니다.
qui 1 echo "hello world"

# qui 3에 명령어를 등록합니다.
qui 5 git add .

# 직전에 썼던 명령어를 qui 4에 등록하고 싶다면
qui 4 !!
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
qui 2
>> Shortcut 3 is not set yet.
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
>>   0: (empty)
>>   1: echo "hello world"
>>   ...
>>   9: (empty)
```
