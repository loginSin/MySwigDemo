package io.rong.imlib.internal.checker;

public class StringChecker {
    private StringChecker() {

    }

    public static boolean isValid(String str) {
        return ObjectChecker.isValid(str) && !str.isEmpty();
    }

    public static boolean isEmpty(String str) {
        return !StringChecker.isValid(str);
    }
}
