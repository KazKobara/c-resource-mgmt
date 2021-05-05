
# c言語の構造体宣言方法 --多数ある理由--

## 基本形

基本は、以下のAを行った後にBを行うことで、struct_instance1, struct_instance2 を利用できるようになる。

- A. 構造体の型の定義

    ```c
    struct struct_type_name_t {
        int16_t i16;
        char   c3[3];
    };
    ```

- B. その構造体の型を持つ変数/インスタンスの宣言

    ```c
    struct struct_type_name_t struct_instance1, struct_instance2;
    ```

## 進化形1 (AとBのまとめ)

AとBはまとめることができる。

- AとBをまとめる場合

    ```c
    struct struct_type_name_t {
        int16_t i16;
        char   c3[3];
    } struct_instance1, struct_instance2;
    ```

- また、この場合は、以下のように構造体の型の名前を省略できる。

    ```c
    struct {
        int16_t i16;
        char   c3[3];
    } struct_instance1, struct_instance2;
    ```

## 進化形2 (typedef の利用)

さらに、typedef を使うと別名への割り振りを行うことができる。

- typedef の使い方

    ```c
    typedef old_name new_name;
    ```

- そのため、Aに対して new_struct_type_name_t という名前を付けることができ、

    ```c
    typedef struct struct_type_name_t {
        int16_t i16;
        char   c3[3];
    } new_struct_type_name_t;
    ```

- この場合についても、以下のように構造体の型の名前を省略できる。

    ```c
    typedef struct {
        int16_t i16;
        char   c3[3];
    } new_struct_type_name_t;
    ```

## 進化形2 (typedef の利用) の最終形

上記において構造体の型の名前はもはや new では無くなったので、new_ を取ると、

- typedef を使う場合の構造体定義の最終形は以下のようになる。

    ```c
    typedef struct {
        int16_t i16;
        char   c3[3];
    } struct_type_name_t;
    ```

- また、この場合の構造体の変数(インスタンス) struct_instance1, struct_instance2 は以下のように宣言することで、利用可能になる。

    ```c
    struct_type_name_t struct_instance1, struct_instance2;
    ```

---

[戻る](README.md)
