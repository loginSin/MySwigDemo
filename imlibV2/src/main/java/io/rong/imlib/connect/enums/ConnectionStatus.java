package io.rong.imlib.connect.enums;

public enum ConnectionStatus {
    Idle(0, ""),
    Connecting(1, ""),
    Connected(2, ""),


    UNKNOWN(9999999, "");

    private int code;
    private String msg;

    ConnectionStatus(int code, String msg) {
        this.code = code;
        this.msg = msg;
    }

    public static ConnectionStatus codeOf(int code) {
        for (ConnectionStatus error : ConnectionStatus.values()) {
            if (code == error.getCode()) {
                return error;
            }
        }

        ConnectionStatus error = UNKNOWN;
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
