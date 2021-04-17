# 型サイズの確認

## 必要な背景知識

- c言語
- ポインタ、構造体

## 型とサイズ

特に long、long long などは処理系で型のサイズが異なるため、sizeof() 関数で確認するためのコードを[こちら](./src/sizeof.c)に置いてある。

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
pointer          : 8
int32_t *ptr     : 4
void *void_ptr   : 1

struct ci16i32   : 8
struct i16i32c   : 12
```

要点としては、

- スタック領域上の配列( int32_t array[9]; など)では sizeof(int32_t) x [配列のサイズ] = 4x9 = 36 となるのに対して、ヒープ領域に動的に割り当てた配列( int32_t \*ptr = (int32_t \*)calloc(9, sizeof(int32_t)); など)では __sizeof(ptr) は ptr のポインターのサイズ__、 __sizeof(\*ptr)はptrが指す値の型のサイズ(型がvoidの場合は1オクテット)__ となるため、注意が必要。
- sizeof(構造体) については、その要素のサイズに応じてアライメントされるため、並びに応じてサイズが変わる場合がある。

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
