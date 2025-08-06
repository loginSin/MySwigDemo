package io.rong.imlib.base.enums;

public enum EngineError {
    Success(0, ""),


    UNKNOWN(9999999, "");

    private int code;
    private String msg;

    EngineError(int code, String msg) {
        this.code = code;
        this.msg = msg;
    }

    public static EngineError codeOf(int code) {
        for (EngineError error : EngineError.values()) {
            if (code == error.getCode()) {
                return error;
            }
        }

        EngineError error = UNKNOWN;
        error.code = code;
        error.msg = code + "";
        return error;
    }

    public int getCode() {
        return this.code;
    }

    public String getMsg() {
        return this.msg;
    }
}
