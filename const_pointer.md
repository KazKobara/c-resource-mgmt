# ポインターに対する const の使い分け

## 必要な背景知識

- c言語
- ポインター
- const

## ポインターに対する const の位置

以下の３通りがある。

- \* の左右両側に const を付ける
  
  \*ptr が指す値を変更する予定は無く、ptr が指すアドレスも変更する予定が無い場合

  ```c
  // 例 二つの例の挙動は同じ
  const uint32_t * const ptr;
  uint32_t const * const ptr;
  ```

- \* の右側のみに const を付ける

  *ptr が指す値を変更する予定はあるが、ptr が指すアドレスは変更する予定が無い場合

  ```c
  // 例
  uint32_t * const ptr;
  ```

- \* の左側のみに const を付ける

  \*ptr が指す値を変更する予定は無いが、ptr が指すアドレスは変更する予定がある場合

  ```c
  // 例 二つの例の挙動は同じ
  const uint32_t * ptr;
  uint32_t const * ptr;
  ```


## 使い分けの例

- \*ptr が配列などの値を表している場合
  - アドレス(ptr)を変更しながら配列の値を参照したり書き換えたりする場合は、\* の右側に const は付けない。ptr を変更しない場合は \* の右側に const を付ける。
  - \*ptr が指している配列の値を書き換えない場合には、\* の左側に const を付ける。
- \*ptr が(配列などではなく)単体の値を指している場合
  - (アドレスの入れ物としての ptr を使いまわしているなどの理由がなければ)原則 \* の右側に const を付けておく。
  - 値(\*ptr)を変更することがなければ、\* の左側にも const を付ける。

## 注意点

```c
    const int32_t constant = 23;        // <--- 固定値constantを23から変更禁止にする
    int32_t*const ptr_vcc = &constant;  // <--- しかしながら、このように宣言してしまうと、*ptr_vcc 経由で書き換え可能になってしまう
```

上記の例ように宣言時に \* の左側に const が付いていないポインター(ptr_vcc)に固定値(constant)のアドレスを入れるとコンパイル時に以下の警告が発せられるが、コンパイルは通り、固定値(constant)の値を \*ptr_vss 経由で変更可能になる。

```text
warning: initialization discards ‘const’ qualifier from pointer target type [-Wdiscarded-qualifiers]
```

固定値(constant)を書き換えてはならない場合には、そのアドレスをコピーするポインター宣言時に以下のように \* の左側に const を付けなければならない。

```c
    const int32_t*const ptr_vcc = &constant; 
```

または、

```c
    int32_t const*const ptr_vcc = &constant; 
```

## 挙動の確認

上記は、以下の実行環境での例ですが、環境により挙動が異なる場合がありますので、
[こちらのソース](./src/const.c)を編集しコンパイルしてみるとよいと思います。

```text
5.4.72-microsoft-standard-WSL2
gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
```

---

[戻る](README.md)
