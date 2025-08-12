package com.ooarchitect.visitingparsetree.nodegenerator.generator;

import java.io.IOException;

abstract class ClosingAppendableWrapper implements AutoCloseableAppendable {

    private final AutoCloseableAppendable wrapped;

    private ClosingAppendableWrapper(AutoCloseableAppendable wrapped) {
        this.wrapped = wrapped;
    }

    @Override
    public Appendable append(CharSequence charSequence) throws IOException {
        return null;
    }

    @Override
    public Appendable append(CharSequence charSequence, int i, int i1) throws IOException {
        return null;
    }

    @Override
    public Appendable append(char c) throws IOException {
        return null;
    }
}
