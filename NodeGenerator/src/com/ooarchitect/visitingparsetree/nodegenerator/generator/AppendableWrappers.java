package com.ooarchitect.visitingparsetree.nodegenerator.generator;

import java.io.IOException;

public class AppendableWrappers {


    /**
     * Wraps {@link Appendable Appendables} that cannot or should not
     * be closed. This lets us use System.out without closing it (which
     * would be very bad), and to treat {@link StringBuffer StringBuffers}
     * as if they <i>could</i> be closed.
     */
    private static class NonClosingWrapper implements AutoCloseableAppendable {

        protected final Appendable wrapped;

        private NonClosingWrapper(Appendable wrapped) {
            this.wrapped = wrapped;
        }

        @Override
        public Appendable append(CharSequence charSequence) throws IOException {
            wrapped.append(charSequence);
            return this;
        }

        @Override
        public Appendable append(CharSequence charSequence, int i, int i1) throws IOException {
            wrapped.append(charSequence, i, i1);
            return this;
        }

        @Override
        public Appendable append(char c) throws IOException {
            wrapped.append(c);
            return null;
        }

        @Override
        public void close() {
        }
    }

    private static class WriterWrapper<T extends AutoCloseable & Appendable> implements AutoCloseableAppendable {
        private final T wrappedWriter;

        private WriterWrapper(T wrapMe) {
            wrappedWriter = wrapMe;
        }

        @Override
        public Appendable append(CharSequence charSequence) throws IOException {
            wrappedWriter.append(charSequence);
            return this;
        }

        @Override
        public Appendable append(CharSequence charSequence, int i, int i1) throws IOException {
            wrappedWriter.append(charSequence, i, i1);
            return this;
        }

        @Override
        public Appendable append(char c) throws IOException {
            wrappedWriter.append(c);
            return this;
        }

        @Override
        public void close() throws Exception {
            wrappedWriter.close();
        }
    }

    static AutoCloseableAppendable ofAppendable(Appendable appendable) {
        return new NonClosingWrapper(appendable);
    }

    static <T extends AutoCloseable & Appendable> AutoCloseableAppendable ofWriter(T writer) {
        return new WriterWrapper<>(writer);
    }
}
