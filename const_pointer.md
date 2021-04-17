# ポインターに対する const の使い分け

## 必要な背景知識

- c言語
- ポインター
- const

## ポインターに対する const の位置

以下の３通りがある。

- \* の左側のみに const を付ける。

```c
const uint32_t * ptr;
uint32_t const * ptr;
```

- \* の右側のみに const を付ける。

```c
uint32_t * const ptr;
```

- \* の左右両側に const を付ける。

```c
const uint32_t * const ptr;
uint32_t const * const ptr;
```

## 使い分けの例

- \*ptr が値を指している場合、\* の右側に const を付ける。
  - さらに、値(\*ptr)を変更することがなければ、\* の左側にも const を付ける。
- \*ptr が配列を表しており、アドレス(ptr)を変更しながら配列の値を参照したり書き換えたりする場合は、\* の右側 const は付けない。ptr を変更しない場合は \* の右側に const を付ける。
  - \*ptr が指している配列の値を書き換えない場合には、\* の左側に const を付ける。

## 挙動の確認

環境により挙動が異なる場合がありますので、
[こちらのソース](./src/const.c)を編集してコンパイルしてみるとよいと思います。

`5.4.72-microsoft-standard-WSL2` 上の `gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0` の挙動の例

```c
    const int32_t constant = 23;        // <--- 書き換え禁止
    int32_t*const ptr_vcc = &constant;  // <--- 書き換え可能になる
```

- 上記のように宣言時に \* の左側に const が付いていないアドレスに const となるべき固定値(constant)のアドレスを入れるとコンパイル時に以下の警告が発せられるが、コンパイルは通り、固定値(constant)の値を *ptr_vss 経由で変更可能になる。
  - 固定値(constant)を書き換えてはならない場合には、そのアドレスをコピーするアドレスの \* の左側に const を付けなければならない。

```text
warning: initialization discards ‘const’ qualifier from pointer target type [-Wdiscarded-qualifiers]
```

---

[戻る](README.md)
