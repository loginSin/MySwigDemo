package io.rong.imlib.message.model;

public class ReceivedInfo {
    private int left;
    private boolean hasPackage;
    private boolean isOffline;

    public int getLeft() {
        return left;
    }

    public void setLeft(int left) {
        this.left = left;
    }

    public boolean isHasPackage() {
        return hasPackage;
    }

    public void setHasPackage(boolean hasPackage) {
        this.hasPackage = hasPackage;
    }

    public boolean isOffline() {
        return isOffline;
    }

    public void setOffline(boolean offline) {
        isOffline = offline;
    }
}
