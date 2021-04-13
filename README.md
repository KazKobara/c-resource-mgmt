# C言語における資源管理

## 必要な背景知識

### 必須

- C言語

### 余裕があれば

- cmake

## 解放時の注意点のまとめ

| func | double release (double free etc.) | null release | 戻り値 |Note |
| :--- | :---: | :---: | :---: | :--- |
| free()   | NG | OK |無||
| fclose() | NG | NG |無||
| close()  | NG\*1 | NG\*2 |有| 戻り値のチェックは必要。 |

- NG: その状況を避けるプログラミングが必要
- OK: プログラミング以外で対処してもらえる
- \*1: 他のスレッドなどがたまたまオープンした file descriptor を close してしまう可能性があるため避けなければならない。
  - open 状態にない file descriptor を close すると戻り値が -1 、 errno が 9 (Bad file descriptor) となる。しかしながら、close() された file descriptor がたまたま他のスレッドなどによりオープンされ、それを close() した場合には、その時点ではエラーとならないため注意が必要。
- \*2: close(0)は標準入力を close するため、それを意図していないなら行ってはならない。
- 動作は以下の環境で確認

    ```text
    Linux 5.4.72-microsoft-standard-WSL2 #1 SMP Wed Oct 28 23:40:43 UTC 2020 x86_64
    cmake version 3.16.3
    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
    ```

## 確認用コード

実行環境により挙動が異なる可能性があるため、実際の挙動を確認するためのコード集。

### 使い方

1. cmake のインストール
    - `apt install cmake` などで

1. build （cmake は省略可）

    ```console
    cd src/build
    cmake ..
    make
    ```

1. 確認用コードの実行

    ```console
    ../check_behavior.sh
    ```

### チェックプログラムを追加した場合

1. CMakeLists.txt に `add_executable(<binary_name_name>, <source_file_name>)` を追加

## Troubleshooting

### cmake で以下のようなエラーが出る場合は `rm ./CMakeCache.txt`

```text
CMake Error: The current CMakeCache.txt directory ...  is different than
```
