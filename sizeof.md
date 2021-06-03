# 型サイズの確認

## 必要な背景知識

- c言語
- ポインタ

## 型とサイズ

処理系で型のサイズが異なる場合(特に long、long long など)や値の範囲が変わる場合(char)があるため、各型のサイズと範囲を確認するコードを[こちら](./src/sizeof.c)に置いてあります。

なお、構造体では要素の順序や pack を行うか否かなどにも依存して、そのサイズが変わるため、詳細につきましては[こちら](struct.md)をご参照下さい。

以下は、

```text
gcc (Ubuntu 9.3.0-17ubuntu1~20.04)
5.4.72-microsoft-standard-WSL2
```

の場合の例:

```text
         Type          :Size [octet]          MIN                  MAX Note
----------------------------------------------------------------------------
                   char:  1                  -128                  127 1
            signed char:  1                  -128                  127
          unsigned char:  1                     0                  255

     (signed) short int:  2                -32768                32767
     unsigned short int:  2                     0                65535
           (signed) int:  4           -2147483648           2147483647
           unsigned int:  4                     0           4294967295
      (signed) long int:  8  -9223372036854775808  9223372036854775807
      unsigned long int:  8                     0 18446744073709551615
 (signed) long long int:  8  -9223372036854775808  9223372036854775807
 unsigned long long int:  8                     0 18446744073709551615

(IEEE754-2008 binary16):( 2)       0.000061035156                65504   3,4
             (bfloat16):( 2)   1.175494350822e-38   3.389531389252e+38 2,3,4
                  float:  4    1.175494350822e-38   3.402823466385e+38 2,3
                 double:  8   2.225073858507e-308  1.797693134862e+308 2,3
            long double: 16  3.362103143112e-4932 1.189731495357e+4932 2,3

              pintptr_t:  8  -9223372036854775808  9223372036854775807
             upintptr_t:  8                     0 18446744073709551615
          pointer (ptr):  8
          int32_t *ptr :  4
        void *void_ptr :  1
       int32_t array[9]: 36

       (signed) ssize_t:  8
      (unsigned) size_t:  8
                 time_t:  8

        struct timespec: 16

Note:
 1. 'char' might be 'unsigned char' in some processing systems, though
    types without 'signed' are the same as 'signed' ones in 'int' types.
 2. MIN and MAX here are rounded.
 3. MIN here is the minimum positive normal value and the smallest value
    is given by -MAX.
 4. Will be updated after implemented in C.
```

```text
gcc version 9.3-win32 20200320 (GCC)
Target: i686-w64-mingw32
```

の場合の例:

```text
         Type          :Size [octet]          MIN                  MAX Note
----------------------------------------------------------------------------
                   char:  1                  -128                  127 1
            signed char:  1                  -128                  127
          unsigned char:  1                     0                  255

     (signed) short int:  2                -32768                32767
     unsigned short int:  2                     0                65535
           (signed) int:  4           -2147483648           2147483647
           unsigned int:  4                     0           4294967295
      (signed) long int:  4           -2147483648           2147483647
      unsigned long int:  4                     0           4294967295
 (signed) long long int:  8  -9223372036854775808  9223372036854775807
 unsigned long long int:  8                     0 18446744073709551615

                  float:  4    1.175494350822e-38   3.402823466385e+38 2,3
                 double:  8   2.225073858507e-308  1.797693134862e+308 2,3
            long double: 12  3.362103143112e-4932 1.189731495357e+4932 2,3

              pintptr_t:  4           -2147483648           2147483647
             upintptr_t:  4                     0           4294967295
          pointer (ptr):  4
          int32_t *ptr :  4
        void *void_ptr :  1
       int32_t array[9]: 36

       (signed) ssize_t:  4
      (unsigned) size_t:  4
                 time_t:  4

        struct timespec:  8
```

```text
gcc version 9.3-win32 20200320 (GCC)
Target: x86_64-w64-mingw32
```

の場合の例:

```text
         Type          :Size [octet]          MIN                  MAX Note
----------------------------------------------------------------------------
                   char:  1                  -128                  127 1
            signed char:  1                  -128                  127
          unsigned char:  1                     0                  255

     (signed) short int:  2                -32768                32767
     unsigned short int:  2                     0                65535
           (signed) int:  4           -2147483648           2147483647
           unsigned int:  4                     0           4294967295
      (signed) long int:  4           -2147483648           2147483647
      unsigned long int:  4                     0           4294967295
 (signed) long long int:  8  -9223372036854775808  9223372036854775807
 unsigned long long int:  8                     0 18446744073709551615

                  float:  4    1.175494350822e-38   3.402823466385e+38 2,3
                 double:  8   2.225073858507e-308  1.797693134862e+308 2,3
            long double: 16  3.362103143112e-4932 1.189731495357e+4932 2,3

              pintptr_t:  8  -9223372036854775808  9223372036854775807
             upintptr_t:  8                     0 18446744073709551615
          pointer (ptr):  8
          int32_t *ptr :  4
        void *void_ptr :  1
       int32_t array[9]: 36

       (signed) ssize_t:  8
      (unsigned) size_t:  8
                 time_t:  8

        struct timespec: 16
```

## 配列のサイズ

sizeof()の引数に何を指定するかにより、得られるサイズが異なるため注意が必要:

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
