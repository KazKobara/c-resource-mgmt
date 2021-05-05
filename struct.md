# 構造体 --漏洩させないための注意点--

## 必要な背景知識

- c言語
- 構造体
  - [c言語の構造体宣言方法 --多数ある理由--](struct_declaration.md)
- [各型のサイズ](sizeof.md)
- alignment
- bit field

## 構造体コピー時の注意点 --メモリ上のデータ漏洩--

コピー元の構造体を `org_struct` 、コピー先の構造体を `dest_struct` とし、 `dest_struct` は
外部からアクセス可能であるとすると、コピー時に気をつけなければならないセキュリティ上の注意点は以下となる。

- `org_struct` の全要素に値が入っていたとしても、 `org_struct` 内に隙間が存在している場合、そこにメモリ上の値が入っている場合があり、`dest_struct = org_struct;` や `memcpy()` などでコピーするとその隙間に入っているメモリ上のデータ毎コピーされ、情報漏洩に繋がる危険性がある。

対策としては、以下などの方法がある。

- dest_struct 全体を隙間を含め**ゼロクリア**し、**要素毎に値をコピー**する。
- org_struct に値を入れる前に隙間を含め全体を**ゼロクリア**しておく。

## もう少し詳しい説明

構造体に限らず初期化されていない変数や領域には、メモリ上に残っているデータが入る。以下、そのようなデータを（重要なデータは残っていないと期待し）ゴミと呼ぶことにする。そのため、初期化されていない値や領域を外部からアクセス可能な場所に移動させたり、複製したりすると、情報漏洩のリスクが生じることになる。

構造体の場合は、その全要素が初期化され、それら全要素に値が代入されていたとしても、構造体要素間や構造体の最終要素の後ろに隙間が存在している場合、それらの場所にゴミが残ることになる。また、構造体の各要素はそのデータ型のサイズに応じてアライメントされたアドレスに配置されるため、多くの場合、隙間を含んでいる。

例えば、[確認用コード](src/struct.c)において、構造体 `org_struct` に以下のような隙間(padding)が入るように各要素を配置し、

```c
typedef struct {        // member_size = type_size * blocks [octet]
    uint8_t b2:2;       // 1 (6 bit padding + 2 bits)
                        // 15 octet padding
    long double f128;   // 16
    uint8_t i8;         // 1
                        // 7 octet padding
    int64_t i64[2];     // 8*2
    int16_t i16;        // 2
                        // 6 octet padding
    double  f64;        // 8
    int32_t i32;        // 4
    char    str[5];     // 1*5
                        // 7 octet padding
    float   f32;        // 4
                        // 15 octet padding
                        // 96 in total
} struct___aligned_unpacked_untruncated_t ;
```

その全要素に以下の値を代入しておく。

```c
struct___aligned_unpacked_untruncated_t org_struct = {1,16.0,1,{8,8},2,8.0,4,"char",4.0};
```

構造体 `org_struct` に関する情報を以下のように表示させると、最右の `value` の列のとおり、全要素に正しく値が入っていることが確認できる。

```c
    Table 1: Properties of org_struct

struct.member : address      size pad al value
--------------:-----------------------------
struct        : 0x7ffc6c528100 96
struct.b2     : 0x????????????  1  15  - 1
struct.f128   : 0x7ffc6c528110 16   0  0 16.000000
struct.i8     : 0x7ffc6c528120  1   7  - 1
struct.i64[2] : 0x7ffc6c528128 16   0  0 {8, 8}
struct.i16    : 0x7ffc6c528138  2   6  0 2
struct.f64    : 0x7ffc6c528140  8   0  0 8.000000
struct.i32    : 0x7ffc6c528148  4   0  0 4
struct.str[5] : 0x7ffc6c52814c  5   3  - "char"
struct.f32    : 0x7ffc6c528154  4   8  0 4.000000
```

Table 1 の `al` の列は、メモリアクセス時のパフォーマンスを上げるためにアライメントされるべきアドレス位置との差を表しており、`0`はそのようにアライメントされていることを意味する。なお、構造体要素の型サイズが 1 octet の場合にはアライメントは関係無いため `-` を表示させてあり、(bit field) b2 の位置はその前後要素の `address` の間のどこかにあるが bit 位置までは表示できないため `0x???...???` と表示してある。

Table 1 の `pad` の列は、その次の要素をアライメントするためにその要素の後ろにできる隙間の octet 数を表している。構造体 `org_struct` のメモリをダンプすると、以下のように `pad` に対応する箇所（例えば、文字列"char\n"の後ろの 7 octets など）にゴミが入っており、[確認用コード](src/struct.c)を実行する度に異なる値が表示される。

```text
A1 96 EB ED E8 7F 00 00 08 16 48 83 19 56 00 00   ..........H..V..
00 00 00 00 00 00 00 80 03 40 EB ED E8 7F 00 00   .........@......
01 00 00 00 00 00 00 00 08 00 00 00 00 00 00 00   ................
08 00 00 00 00 00 00 00 02 00 EB ED E8 7F 00 00   ................
00 00 00 00 00 00 20 40 04 00 00 00 63 68 61 72   ...... @....char
00 04 48 83 00 00 80 40 80 81 52 6C FC 7F 00 00   ..H....@..Rl....
```

なお、コピー先の構造体 `dest_struct` にも初期化前にはメモリ上のゴミが入っているが、ゼロクリア後は、以下のとおり `0` の値で上書きされる。

```text
0x7ffc6c5280a0    dest_struct after zero-cleared
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
```

しかしながら、ゴミが入ったままの `org_struct` を `dest_struct = org_struct;` (や `memcpy()` など)で（外部から読み取り可能な）構造体 `dest_struct` にコピーしてしまうと、以下のとおりゴミもろともコピーされてしまう。

```text
0x7ffc6c5280a0    dest_struct = org_struct
A1 96 EB ED E8 7F 00 00 08 16 48 83 19 56 00 00   ..........H..V..
00 00 00 00 00 00 00 80 03 40 EB ED E8 7F 00 00   .........@......
01 00 00 00 00 00 00 00 08 00 00 00 00 00 00 00   ................
08 00 00 00 00 00 00 00 02 00 EB ED E8 7F 00 00   ................
00 00 00 00 00 00 20 40 04 00 00 00 63 68 61 72   ...... @....char
00 04 48 83 00 00 80 40 80 81 52 6C FC 7F 00 00   ..H....@..Rl....
```

対策の一つは前述のとおり、`dest_struct = org_struct;` (や `memcpy()` など)で全体をコピーするのではなく、
（`dest_struct` の隙間は既にゼロクリアされているため）要素のみコピーするか、`org_struct` 全体をゼロクリアした後に必要な要素に値を入れてから、`dest_struct = org_struct;` などでコピーすることである。

なお、bit field には有効ではないが構造体中の padding を無くす（または、padding となる部分にダミーの要素を入れ初期化しておく）という方法もあるが、アライメントが崩れないように要素の配置順序にも配慮する必要がある。詳しくは以下で説明する。

## 詰め物(padding)を削除すべきか整列(alignment)を保つべきか

構造体に限った話ではないが、一般的に 2 octets 以上のサイズを持つ型のデータは、その物理アクセスを効率化するために一定の規則に従ったアドレスに配置される。この処理はアライメントと呼ばれる。

例えば、上記 Table 1 の `org_struct` の `address` の列に示すとおり、型のsize が 16 octets である long double型の変数 f128 は、その `address` の最小 4 bits (ここで 4 は型のサイズ 16 の log_2、つまり 4=log2 16 )の値が16進数で `0` となる場所に配置され、型のsize が 8 octets である (int64_t) i64, (double) f64 は `address` の最小 3 (=log_2 8) bits の値が `0` (すなわち、16進数では `0` または `8`)となる場所に配置される。また、そのように配置されるように、直前の要素の後ろに同表の `pad` 列に示されている octets 分の padding が行われる。

なお、padding 自体は構造体を定義する際に、例えば以下などのように `__attribute__((__packed__))` を付けたり、`#pragma pack` で囲んだりすることで削除できるが、構造体要素の順序が配慮されていない場合にはアライメントがずれてしまう。

```c
/* Bad Example */
typedef struct {            // member_size = type_size * blocks [octet]
    uint8_t b2:2;           // 1 (6 bit padding + 2 bits)
    long double f128;       // 16
    uint8_t i8;             // 1
    int64_t i64[2];         // 8*2
    int16_t i16;            // 2
    double  f64;            // 8
    int32_t i32;            // 4
    char    str[5];         // 1*5
    float   f32;            // 4
                            // 57 in total
} __attribute__((__packed__)) struct_unaligned___packed___truncated_t ;

/* Bad Example */
#pragma pack(push, 1)
typedef struct {            // member_size = type_size * blocks [octet]
    uint8_t b2:2;           // 1 (6 bit padding + 2 bits)
    long double f128;       // 16
    uint8_t i8;             // 1
    int64_t i64[2];         // 8*2
    int16_t i16;            // 2
    double  f64;            // 8
    int32_t i32;            // 4
    char    str[5];         // 1*5
    float   f32;            // 4
                            // 57 in total
} struct_unaligned___packed___truncated_with_pragma_t ;
#pragma pack(pop)
```

実際に、末尾に示す実行環境でこれらのアドレスなどの情報を表示させたのが以下の表である。pack処理の結果、詰め物は削除され `pad` の列は `0` となり、構造体のサイズも 96 octets から 57 octets に削減されたが、alignment がずれ `al` の(`-`以外の)列において `0` でない値が現れていることが分かる。

```c
------- struct_unaligned___packed___truncated_t -------
struct.member : address      size pad al value
--------------:-----------------------------
struct        : 0x7ffecb9c2a00 57
struct.b2     : 0x????????????  1   0  - 0
struct.f128   : 0x7ffecb9c2a01 16   0  1 nan
struct.i8     : 0x7ffecb9c2a11  1   0  - 22
struct.i64[2] : 0x7ffecb9c2a12 16   0  2 {2814751909664, 1695605259704991}
struct.i16    : 0x7ffecb9c2a22  2   0  0 -10633
struct.f64    : 0x7ffecb9c2a24  8   0  4 -123978158535939934115704.000000
struct.i32    : 0x7ffecb9c2a2c  4   0  0 32692
struct.str[5] : 0x7ffecb9c2a30  5   0  - ".4.....@4GV"
struct.f32    : 0x7ffecb9c2a35  4   0  1 -55392225926982926336.000000

------- struct_unaligned___packed___truncated_with_pragma -------
struct.member : address      size pad al value
--------------:-----------------------------
struct        : 0x7ffecb9c2a40 57
struct.b2     : 0x????????????  1   0  - 0
struct.f128   : 0x7ffecb9c2a41 16   0  1 nan
struct.i8     : 0x7ffecb9c2a51  1   0  - 70
struct.i64[2] : 0x7ffecb9c2a52 16   0  2 {966022121518497, 1774418253183975}
struct.i16    : 0x7ffecb9c2a62  2   0  0 -10633
struct.f64    : 0x7ffecb9c2a64  8   0  4 -12940035658227818032904259.000000
struct.i32    : 0x7ffecb9c2a6c  4   0  0 32692
struct.str[5] : 0x7ffecb9c2a70  5   0  - "h"
struct.f32    : 0x7ffecb9c2a75  4   0  1 0.000000
```

## 構造体内の詰め物(padding)を削除し、かつ、整列(align)させる方法

アライメントがずれてしまっても、パフォーマンスが重要でなければあまり気にする必要はないが、
構造体内の詰め物(padding)を削除し、かつ、整列(align)させることは以下の方法により行える。

1. 構造体の要素をデータ型のサイズが大きい順に並べる。
   - 例:

    ```c
    /* Still not a good example */
    typedef struct {        // member_size = type_size * blocks [octet]
        long double f128;   // 16
        int64_t i64[2];     // 8*2
        double  f64;        // 8
        int32_t i32;        // 4
        float   f32;        // 4
        int16_t i16;        // 2
        uint8_t i8;         // 1
        uint8_t b2:2;       // 1 (6 bits padding + 2 bits)
        char    str[S5];    // 1*5
                            // 7 octet padding
                            // 64 in total
    } struct___aligned___packed_untruncated_t;
    ```

   - ただし、ここでのサイズは、構造体要素のサイズではなく、要素を構成するデータの型のサイズである。
     - 例えば、上記の例だと i64[2] という構造体要素のサイズは 16 = 8*2 octets であるが、そのサイズではなく i64 の型である int64_t のサイズ 8 octets。
     - 環境によりサイズが変わる場合には、サイズが変わっても大きさの順序が変わらな順序で配置しておくか、bit幅指定の型(int64_tなど)を用いる。
   - もし、2のべき乗ではないサイズが存在する場合には、別の要素と組み合わせるか、初期化されたダミーの要素を追加し、2のべき乗にしておく。
   - 上記により定義された構造体の `address` などを表示させると alignment の列 `al` は `0` となり、`pad` の列も最後尾を除き `0` となる。

    ```c
    ------- struct___aligned___packed_untruncated_t -------
    struct.member : address      size pad al value
    --------------:-----------------------------
    struct        : 0x7fffc15b2a60 64
    struct.f128   : 0x7fffc15b2a60 16   0  0 nan
    struct.i64[2] : 0x7fffc15b2a70 16   0  0 {61, 94819633517216}
    struct.f64    : 0x7fffc15b2a80  8   0  0 0.000000
    struct.i32    : 0x7fffc15b2a88  4   0  0 -821921071
    struct.f32    : 0x7fffc15b2a8c  4   0  0 0.000000
    struct.i16    : 0x7fffc15b2a90  2   0  0 2208
    struct.i8     : 0x7fffc15b2a92  1   0  - 24
    struct.b2     : 0x????????????  1   0  - 3
    struct.str[5] : 0x7fffc15b2a94  5   7  - ""
    ```

1. 上記により残った最後尾の padding を pack で取り除く。
   - 例えば、構造体定義時に `__attribute__((__packed__))` を付けたり、`#pragma pack` で囲むなど。

    ```c
    typedef struct {            // member_size = type_size * blocks [octet]
        long double f128;       // 16
        int64_t i64[2];         // 8*2
        double  f64;            // 8
        int32_t i32;            // 4
        float   f32;            // 4
        int16_t i16;            // 2
        uint8_t i8;             // 1
        uint8_t b2:2;           // 1 (6 bits padding + 2 bits)
        char    str[5];         // 1*5
                                // 57 in total
    } __attribute__((__packed__)) struct___aligned___packed___truncated_t;

    #pragma pack(push, 1)
    typedef struct {            // member_size = type_size * blocks [octet]
        long double f128;       // 16
        int64_t i64[I64_LEN];   // 8*2
        double  f64;            // 8
        int32_t i32;            // 4
        float   f32;            // 4
        int16_t i16;            // 2
        uint8_t i8;             // 1
        uint8_t b2:2;           // 1 (6 bits padding + 2 bits)
        char    str[STR_LEN];   // 1*5
                                // 57 in total
    } struct___aligned___packed___truncated_with_pragma_t;
    #pragma pack(pop)
    ```

    - これにより、最後尾の padding も `0` となる。

    ```c
    ------- struct___aligned___packed___truncated_t -------
    struct.member : address      size pad al value
    --------------:-----------------------------
    struct        : 0x7fffc15b29a0 57
    struct.f128   : 0x7fffc15b29a0 16   0  0 nan
    struct.i64[2] : 0x7fffc15b29b0 16   0  0 {4616189618054758400, 10}
    struct.f64    : 0x7fffc15b29c0  8   0  0 0.000000
    struct.i32    : 0x7fffc15b29c8  4   0  0 -820508792
    struct.f32    : 0x7fffc15b29cc  4   0  0 0.000000
    struct.i16    : 0x7fffc15b29d0  2   0  0 30016
    struct.i8     : 0x7fffc15b29d2  1   0  - 24
    struct.b2     : 0x????????????  1   0  - 3
    struct.str[5] : 0x7fffc15b29d4  5   0  - ""

    ------- struct___aligned___packed___truncated_with_pragma_t -------
    struct.member : address      size pad al value
    --------------:-----------------------------
    struct        : 0x7ffd3fe36e20 57
    struct.f128   : 0x7ffd3fe36e20 16   0  0 -nan
    struct.i64[2] : 0x7ffd3fe36e30 16   0  0 {94053240661781, 3432}
    struct.f64    : 0x7ffd3fe36e40  8   0  0 0.000000
    struct.i32    : 0x7ffd3fe36e48  4   0  0 -1340558940
    struct.f32    : 0x7ffd3fe36e4c  4   0  0 0.000000
    struct.i16    : 0x7ffd3fe36e50  2   0  0 3432
    struct.i8     : 0x7ffd3fe36e52  1   0  - 0
    struct.b2     : 0x????????????  1   0  - 0
    struct.str[5] : 0x7ffd3fe36e54  5   0  - ""
    ```

## 構造体のサイズ -- 同一要素を持つ構造体のサイズの違い --

最後に、[確認用コード](src/struct.c)で定義した6種類の構造体の一覧とそのサイズを以下にまとめておく。これら6種類の構造体の要素は全て同じであるが、その配置順や pack 処理の有無によりサイズが異なる。Size は小さい方が、構造体内の隙間が狭く、かつ、構造体の名前が \*\_unaligned\_\* ではなく \*\_aligned\_\* となっている方がパフォーマンス的には好ましいことになる。

```c
              Type of struct                        : Size [octet]
---------------------------------------------------------------------
struct___aligned_unpacked_untruncated_t             : 96
struct_unaligned___packed___truncated_t             : 57
struct_unaligned___packed___truncated_with_pragma_t : 57
struct___aligned___packed_untruncated_t             : 64
struct___aligned___packed___truncated_t             : 57
struct___aligned___packed___truncated_with_pragma_t : 57
```

## 動作確認環境

```text
Linux 5.4.72-microsoft-standard-WSL2 #1 SMP Wed Oct 28 23:40:43 UTC 2020 x86_64
gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
```

---

[戻る](README.md)
