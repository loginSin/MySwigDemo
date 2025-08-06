package io.rong.imlib.connect.listener;

import io.rong.imlib.connect.enums.ConnectionStatus;

public interface ConnectionStatusListener {
    void onConnectionStatusChanged(ConnectionStatus status);
}
