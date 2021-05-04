# 型サイズの確認

## 必要な背景知識

- c言語
- ポインタ

## 型とサイズ

処理系で型のサイズが異なる場合がある(特に long、long long など)ため、各型のサイズを確認するコードを[こちら](./src/sizeof.c)に置いてあります。

構造体のサイズは、条件により変わるため[こちら](struct.md)をご参照下さい。

以下は、

```text
gcc (Ubuntu 9.3.0-17ubuntu1~20.04)
5.4.72-microsoft-standard-WSL2
```

の場合の例:

```text
Type             : Size in Octets
----------------------------------
char             : 1
short int        : 2
int              : 4
long int         : 8
long long int    : 8
float            : 4
double           : 8
long double      : 16

int32_t array[9] : 36
pointer (ptr)    : 8
int32_t *ptr     : 4
void *void_ptr   : 1
```

## 配列のサイズ

sizeof()の引数により異なるため注意が必要:

- __スタック領域__ 上の配列( int32_t array[9]; など)では [配列の型のサイズ] x [配列のサイズ]
  - 上記の例では、sizeof(array) = sizeof(int32_t) x [配列のサイズ] = 4x9 = 36。
- __ヒープ領域__ に動的に割り当てた配列( int32_t \*ptr = (int32_t \*)calloc(9, sizeof(int32_t)); など)
  - sizeof(ptr) は ptr のポインターのサイズ
  - sizeof(\*ptr)はptrが指す値の型のサイズ(型がvoidの場合は1オクテット)

## サイズを考慮した型

- 処理系によりサイズが変わっては困る場合には、 <stdint.h> などで定義されている
[ビット幅制限型](https://www.jpcert.or.jp/sc-rules/c-int00-c.html)(例えば、int32_t, uint32_t, int_fast32_t)などを用いる。
  - なお、u_int8_t, u_int16_t, u_int32_t, u_int64_t などは、GNU-Linux platform の <sys/types.h> で定義されており [portable ではない](https://stackoverflow.com/questions/42979055/should-i-use-c-types-uint8-t-uint64-t-or-u-int8-t-u-int64-t)とのこと。

## main()関数の戻り値の型

- 多くの関数の戻り値に合わせるなら `int` などだが、ビット幅を指定する場合には `int32_t` など。
- シェルやコマンドプロンプトでの戻り値に合わせるなら `uint8_t` など。
  - なお、linux のシェルでは戻り値は 0-255 でそれ以外の値は 256 で割った値 (-1の場合は255) が返される。

---

[戻る](README.md)
