package com.ooarchitect.visitingparsetree.nodegenerator.generator;

/**
 * An agglomeration of AutoCloseable and Appendable that
 * generator uses to manage output streams.
 */
interface AutoCloseableAppendable
        extends AutoCloseable, Appendable{
}
