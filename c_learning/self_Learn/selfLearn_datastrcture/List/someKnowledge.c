#include<stdio.h>
#include<stdlib.h>
/*  1. 局部结构体变量（函数内定义）
        存储位置：栈区。
        特点：当函数执行时，结构体变量在栈上被创建；函数结束后，栈内存被自动释放。
e.g.:
    void func() {
    struct MyStruct s; // 栈上分配
    // ... 使用 s
} // s 在此处被销毁
    2.全局结构体变量(直接在所有函数外面使用结构体进行定义)
        存储位置：静态存储区。
        特点：在程序启动时分配，程序结束后释放，生命周期贯穿整个程序。
    3.动态分配的结构体变量（通过 malloc）
        存储位置：堆区。
        特点：需手动调用 malloc 分配内存，调用 free 释放。若未释放，会导致内存泄漏。
e.g.:
    struct MyStruct* ptr = malloc(sizeof(struct MyStruct)); // 堆上分配
    free(ptr); // 释放内存
    4.返回局部结构体变量（栈上）:
        存储位置：函数返回时，结构体的副本会被复制到调用者的栈帧中。
        注意：原局部变量（栈上）在函数结束后被销毁，但返回的是副本，因此调用者可以安全使用。
        示例：
        struct MyStruct func() {
            struct MyStruct s; // 栈上
            s.data = 10;
            return s; // 返回副本到调用者栈区
        }

        int main() {
            struct MyStruct result = func(); // result 在 main 函数的栈区
            return 0;
        }
    5.返回结构体指针（需注意生命周期）
        返回栈上的指针：危险！指针指向的内存已被释放，导致未定义行为。
        struct MyStruct* func() {
            struct MyStruct s; // 栈上
            return &s; // 错误：返回栈内存地址;因为s是在函数内定义的，在函数结束时就已经
            //被释放了，也就是返回了一个已经被释放的地址
        }
        
        返回堆上的指针：安全，但需手动释放。
        struct MyStruct* func() {
            struct MyStruct* ptr = malloc(sizeof(struct MyStruct)); // 堆上
            //因为分配在堆上，所以在函数结束时不会被释放
            ptr->data = 10;
            return ptr; // 正确
        //这个返回的是指针
        }
        int main() {
            struct MyStruct* result = func();
            free(result); // 必须释放
            return 0;
        }
    6.结构体包含指针成员的特殊情况
        若结构体包含指针（如动态分配的内存），返回结构体时需注意：
        指针成员的指向：指针本身会被复制，但指向的内存不会自动复制。
        深拷贝与浅拷贝：若需独立管理内存，需手动深拷贝指针指向的数据。
        示例：
        struct MyStruct {
            int* ptr;
        };

        struct MyStruct func() {
            struct MyStruct s;
            s.ptr = malloc(sizeof(int)); // 堆上
            *s.ptr = 10;
            return s; // 返回副本，但 ptr 仍指向同一块堆内存
            //这只是将指针的值复制到了新的结构体副本上，而并不是复制指针所指向的内存内容
//函数返回的是一个结构体，在返回时会进行结构体的复制。结构体中包含一个指针成员，复制时只是复制了指针的值（地址），
而不是指针指向的内存内容，从而产生了浅拷贝问题。虽然多了先定义结构体再为其指针成员分配内存这一步，
但关键在于返回的是结构体本身，导致了复制操作的发生，与返回指针的情况不同。
        }
        int main() {
            struct MyStruct result = func();
            *result.ptr = 20; // 会影响原 s.ptr 的值（浅拷贝）
            free(result.ptr); // 正确释放
            return 0;
        }
*/  

/*下面详细分析在主函数里使用 `typedef` 的结构体通过不同方式创建和初始化变量时，在内存存储等方面的区别。

### 1. 准备工作：定义结构体
假设我们有如下 `typedef` 定义的结构体：
```c
#include <stdio.h>
#include <stdlib.h>

// 定义结构体并使用 typedef 重命名
typedef struct {
    int data;
    char name[20];
} MyStruct;
```

### 2. 不同创建和初始化方式及内存存储区别

#### 方式一：在主函数里直接创建结构体变量并初始化
```c
int main() {
    // 直接创建结构体变量
    MyStruct s;
    // 初始化结构体成员
    s.data = 10;
    sprintf(s.name, "example");

    // 操作结构体
    printf("Data: %d, Name: %s\n", s.data, s.name);

    return 0;
}
```
- **内存存储位置**：变量 `s` 存储在栈上。栈是由操作系统自动管理的内存区域，当函数（这里是 `main` 函数）执行结束时，栈上的变量会自动被销毁，其占用的内存会被回收。
- **特点**：这种方式简单直接，适合创建生命周期较短、大小固定的结构体变量。但由于栈空间有限，如果创建过大的结构体数组等可能会导致栈溢出。

#### 方式二：在主函数里使用 `malloc` 动态分配内存来创建结构体变量
```c
int main() {
    // 使用 malloc 动态分配内存
    MyStruct *s = (MyStruct *)malloc(sizeof(MyStruct));
    if (s == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    // 初始化结构体成员
    s->data = 20;
    sprintf(s->name, "dynamic");

    // 操作结构体
    printf("Data: %d, Name: %s\n", s->data, s->name);

    // 释放动态分配的内存
    free(s);

    return 0;
}
```
- **内存存储位置**：变量 `s` 本身存储在栈上，它是一个指针，指向在堆上动态分配的 `MyStruct` 类型的内存空间。
堆是由程序员手动管理的内存区域，需要使用 `malloc`、`calloc` 等函数分配内存，使用 `free` 函数释放内存。
- **特点**：动态分配内存可以在运行时根据需要调整内存大小，适合创建生命周期较长、大小不确定的结构体变量。
但需要程序员手动管理内存，若忘记释放内存会导致内存泄漏。

#### 方式三：使用 `create` 函数创建结构体变量
```c
// create 函数
MyStruct* createMyStruct() {
    MyStruct *s = (MyStruct *)malloc(sizeof(MyStruct));
    if (s == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    // 初始化结构体成员
    s->data = 30;
    sprintf(s->name, "created");
    return s;
}
int main() {
    // 使用 create 函数创建结构体变量
    MyStruct *s = createMyStruct();
    if (s != NULL) {
        // 操作结构体
        printf("Data: %d, Name: %s\n", s->data, s->name);
        // 释放动态分配的内存
        free(s);
    }

    return 0;
}
```
- **内存存储位置**：在 `createMyStruct` 函数中，使用 `malloc` 在堆上分配了 `MyStruct` 类型的内存空间，函数返回指向该内存空间的指针。
在 `main` 函数中，变量 `s` 存储在栈上，它接收了 `createMyStruct` 函数返回的指针，指向堆上的内存。
- **特点**：这种方式将结构体的创建和初始化封装在一个函数中，提高了代码的复用性和可维护性。同样，需要程序员手动管理堆上的内存，避免内存泄漏。
### 总结
- **栈上直接创建**：简单方便，适合短期使用、大小固定的情况，由系统自动管理内存。
- **`malloc` 动态分配**：灵活可变，适合长期使用、大小不确定的情况，需要手动管理内存。
- **`create` 函数创建**：封装性好，将创建和初始化逻辑封装在函数中，同样需要手动管理内存。 */
/*下面结合使用初始化函数、`malloc` 分配内存以及 `create` 函数创建结构体这几种方式，详细分析它们在内存存储等方面的区别。

### 前提：定义结构体
假设我们有如下 `typedef` 定义的结构体：
```c
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int data;
    char name[20];
} MyStruct;
```

### 方式一：主函数里创建结构体变量并用初始化函数初始化
```c
// 初始化函数
void initMyStruct(MyStruct *s) {
    s->data = 10;
    sprintf(s->name, "initialized");
}

int main() {
    // 在主函数栈上创建结构体变量
    MyStruct s;
    // 使用初始化函数初始化
    initMyStruct(&s);

    printf("Data: %d, Name: %s\n", s.data, s.name);

    return 0;
}
```
- **内存存储位置**：
    - 结构体变量 `s` 存储在栈上。栈是由操作系统自动管理的内存区域，当 `main` 函数执行结束，`s` 所占用的内存会自动被回收。
    - 初始化函数 `initMyStruct` 接收的是 `s` 的地址，它直接在栈上的 `s` 结构体上进行操作，不会产生额外的内存分配。
- **特点**：
    - 操作简单直接，适合创建生命周期较短、大小固定的结构体变量。
    - 由于栈空间有限，如果创建过大的结构体数组等可能会导致栈溢出。

### 方式二：主函数里使用 `malloc` 分配内存并初始化
```c
int main() {
    // 使用 malloc 在堆上分配内存
    MyStruct *s = (MyStruct *)malloc(sizeof(MyStruct));
    if (s == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    // 直接初始化
    s->data = 20;
    sprintf(s->name, "dynamic");

    printf("Data: %d, Name: %s\n", s->data, s->name);

    // 释放动态分配的内存
    free(s);

    return 0;
}
```
- **内存存储位置**：
    - 指针变量 `s` 存储在栈上，它指向的 `MyStruct` 类型的内存空间是在堆上动态分配的。堆是由程序员手动管理的内存区域，需要使用 `malloc` 分配内存，使用 `free` 释放内存。
- **特点**：
    - 动态分配内存可以在运行时根据需要调整内存大小，适合创建生命周期较长、大小不确定的结构体变量。
    - 需要程序员手动管理内存，若忘记释放内存会导致内存泄漏。

### 方式三：使用 `create` 函数创建并初始化结构体
```c
// create 函数
MyStruct* createMyStruct() {
    // 在堆上分配内存
    MyStruct *s = (MyStruct *)malloc(sizeof(MyStruct));
    if (s == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    // 初始化结构体成员
    s->data = 30;
    sprintf(s->name, "created");
    return s;
}

int main() {
    // 使用 create 函数创建并初始化结构体
    MyStruct *s = createMyStruct();
    if (s != NULL) {
        printf("Data: %d, Name: %s\n", s->data, s->name);
        // 释放动态分配的内存
        free(s);
    }

    return 0;
}
```
- **内存存储位置**：
    - 在 `createMyStruct` 函数中，使用 `malloc` 在堆上分配了 `MyStruct` 类型的内存空间，函数返回指向该内存空间的指针。
    - 在 `main` 函数中，指针变量 `s` 存储在栈上，它接收了 `createMyStruct` 函数返回的指针，指向堆上的内存。
- **特点**：
    - 这种方式将结构体的创建和初始化封装在一个函数中，提高了代码的复用性和可维护性。
    - 同样需要程序员手动管理堆上的内存，避免内存泄漏。

### 总结
| 方式 | 内存存储位置 | 生命周期管理 | 特点 |
| ---- | ---- | ---- | ---- |
| 主函数创建 + 初始化函数 | 结构体在栈上，初始化函数操作栈上结构体 | 系统自动管理，函数结束自动释放 | 简单直接，适合短期、固定大小场景，栈空间有限 |
| 主函数 `malloc` 分配并初始化 | 指针在栈上，指向堆上内存 | 手动管理，需 `free` 释放 | 灵活可变，适合长期、大小不确定场景，易内存泄漏 |
| `create` 函数创建并初始化 | 指针在栈上，指向堆上内存 | 手动管理，需 `free` 释放 | 封装性好，提高代码复用和可维护性，易内存泄漏 | */