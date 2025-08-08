
// 告诉 SWIG 把 RcimConversationType 当作 int 处理，注意，声明的枚举值不能带 enum 关键字
%apply int { RcimConversationType };
%apply int { RcimMessageDirection };
%apply int { RcimSentStatus };
%apply int { RcimPlatform };

/* 生成的 Java 类覆盖析构方法 */
%typemap(javacode) SWIGTYPE %{
/**
 * 该方法由配置生成<p>
 * delete() 方法名太过于普通，所以新增 swigDelete()<p>
 * 使用该方法进行释放，不要使用 delete()<p>
 * 方便进行代码创建、释放的检查
 */
public synchronized void swigDelete() {
    delete();
}

/**
 * 该方法由配置生成<p>
 * WARNING： 原则上不能调用该方法，除非你明确知道你需要这个方法<p>
 * 静态工厂方法：从 C 指针构建该对象
 */
public static $javaclassname fromPointer(long cPtr) {
    return new $javaclassname(cPtr, false);
}

/**
 * 该方法由配置生成<p>
 * WARNING： 原则上不能调用该方法，除非你明确知道你需要这个方法<p>
 * 获取原始指针
 */
public long getCPtr() {
    return swigCPtr;
}
%}


%include "extracted_struct_define.h"
