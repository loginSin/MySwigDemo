package io.rong.imlib.internal.checker;

import androidx.annotation.Nullable;

public class ObjectChecker {
    private ObjectChecker() {

    }

    public static boolean isValid(@Nullable Object obj) {
        return obj != null;
    }
}
