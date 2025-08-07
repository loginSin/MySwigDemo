
// 告诉 SWIG 把 RcimConversationType 当作 int 处理，注意，声明的枚举值不能带 enum 关键字
%apply int { RcimConversationType };
%apply int { RcimMessageDirection };
%apply int { RcimSentStatus };
%apply int { RcimPlatform };

%typemap(javacode) RcimMessageBox %{
/** 静态工厂方法：从 C 指针构建 RcimMessageBox 对象 */
public static RcimMessageBox fromPointer(long cPtr) {
    return new RcimMessageBox(cPtr, false);
}
%}

%typemap(javacode) RcimSDKVersion %{
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
