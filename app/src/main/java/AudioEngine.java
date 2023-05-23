

public class AudioEngine {

    static {
        System.loadLibrary("native-lib");
    }

    private long mEngineHandle;

    public AudioEngine() {
        mEngineHandle = createEngine();
    }

    public void startStream() {
        startStream(mEngineHandle);
    }

    public void stopStream() {
        stopStream(mEngineHandle);
    }

    private native long createEngine();
    private native void deleteEngine(long engineHandle);
    private native void startStream(long engineHandle);
    private native void stopStream(long engineHandle);

    @Override
    protected void finalize() throws Throwable {
        deleteEngine(mEngineHandle);
        super.finalize();
    }
}
