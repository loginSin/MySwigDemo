
// 告诉 SWIG 把 RcimConversationType 当作 int 处理，注意，声明的枚举值不能带 enum 关键字
%apply int { RcimConversationType };
%apply int { RcimMessageDirection };
%apply int { RcimSentStatus };
%apply int { RcimPlatform };

/* 生成的 Java 类覆盖析构方法 */
%typemap(javacode) SWIGTYPE %{
/**
 * delete() 方法名太过于普通，所以新增 swigDelete()
 * 使用该方法进行释放，不要使用 delete()
 * 方便进行代码释放的检查
 * */
public synchronized void swigDelete() {
    delete();
}
%}

%typemap(javacode) RcimSDKVersion %{
public synchronized void swigDelete() {
    delete();
}
/** 静态工厂方法：从 C 指针构建 RcimMessageBox 对象 */
public static RcimSDKVersion fromPointer(long cPtr) {
    return new RcimSDKVersion(cPtr, false);
}

/** 获取原始指针 */
public long getCPtr() {
    return swigCPtr;
}
%}

%include "extracted_struct_define.h"
