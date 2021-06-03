# C言語における資源解放時の微妙な違い

## 必要な背景知識

### 必須

- C言語

### 余裕があれば

- cmake

## 資源解放時の微妙な違いのまとめ

以下の表の挙動は以下の環境のもの

  ```text
  Linux 5.4.72-microsoft-standard-WSL2 #1 SMP Wed Oct 28 23:40:43 UTC 2020 x86_64
  cmake version 3.16.3
  gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
  ```

| function | double release (double free etc.) | null release | 戻り値 |Note |
| :--- | :---: | :---: | :---: | :--- |
| free()   | NG | OK |無||
| fclose() | NG | NG |無|\*3|
| close()  | NG\*1 | NG\*2 |有| \*3、*4 |

- NG: その状況を避けるプログラミングが必要
- OK: プログラミングによる対処は不要
- \*1: 他のスレッドなどがたまたまオープンした file descriptor (以下fd)を close してしまう可能性があるため double close() はプログラミングにより回避しなければならない。
  - double close となっていないことを確認するか、close()後は`fd=-1`とし、`fd==-1`の場合には close() しないようにする。
  - open 状態にない fd を close すると戻り値が -1 、 errno が 9 (EBADF : Bad file descriptor) となる。そのため、通常は double close() はエラーになるが、close() された fd がたまたま他のスレッドなどによりオープンされ、それを close() した場合には、その時点ではエラーとならないため上記のような対応が必要。
  - shell から起動されるプロセス一つ当たりが open 可能な fd の数は `ulimit -n` コマンドで確認および設定可能。一般/特権ユーザが設定可能な上限は、それぞれ `ulimit -Sn`、`ulimit -Hn` コマンドで確認できる。
  - 起動プロセスのそれは `cat /proc/${PID}/limits | grep "open file"` で確認できる。
  - プロセスが使用している df は `sudo ls -l /proc/${PID}/fd` で確認できる。
- \*2: close(0)は標準入力を close するため、標準入力を close することを意図していないなら行ってはならない。
- \*3: TOCTOU race condition 攻撃を避けるために open() 後に fdopen() を行う場合、if (NULL == fdopen()) の場合は、その時点で close() でファイルを閉じる。それ以外は、ファイル操作が不要になった時点で fclose() のみを実行する（close()も自動的に実行される）。
  - fclose() と close() の両方を実行することは close() を2回実行することになるため不可。
- \*4: close() の戻り値のチェックは行っておいた方がよい。特に write(2) が NFS やディスククォータが使用される環境において行われる場合。また、write(2)したデータが物理的に保存されることを保証する必要がある場合には fsync(2) も実行しておく([Linux Man Page CLOSE(3P)][LMP] [(和訳)][LMP-jp]より)

[LMP]: https://man7.org/linux/man-pages/man2/close.2.html
[LMP-jp]: https://linuxjm.osdn.jp/html/LDP_man-pages/man2/close.2.html

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

---

[戻る](README.md)
