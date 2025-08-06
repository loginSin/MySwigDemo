
/* example.i */
%module example  // 生成的 Java 类名

%{
/* 包含原始 C 头文件（如有） */
extern int add(int a, int b);
%}

/* 声明要包装的函数 */
extern int add(int a, int b);