package org.ooarchitect.visitingparsetree.nodegenerator.generator;

import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.CharStreams;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;

public class NodeGenerator {
    public static class Builder {
        private CharStream input;
        private Appendable cppOutput;
        private Appendable headerOutput;

        private Builder() {
            input = null;
            cppOutput = null;
            headerOutput = null;
        }

        public Builder inputFromString(String inputString) {
            input = CharStreams.fromString(inputString);
            return this;
        }

        public Builder inputFromFile(String fileName)
                throws IOException {
            input = CharStreams.fromFileName(fileName);
            return this;
        }

        public Builder outputToFile(String fileName)
                throws IOException {
            FileOutputStream cppStream = new FileOutputStream(fileName + ".cpp");
            cppOutput = new OutputStreamWriter(cppStream);
            FileOutputStream headerStream = new FileOutputStream(fileName + ".h");
            headerOutput = new OutputStreamWriter(headerStream);
            return this;
        }

        public Builder outputToStdout() {
            cppOutput = System.out;
            headerOutput = System.out;
            return this;
        }

        public Builder outputToString(StringBuilder output) {
            this.cppOutput = output;
            this.headerOutput = output;
            return this;
        }

        public NodeGenerator build() {
            return new NodeGenerator(input, cppOutput, headerOutput);
        }
    }

    public static Builder builder() {
        return new Builder();
    }

    private static final String DEFAULT_BASE_CLASS = "BaseAstNode";

    private final CharStream input;
    private final Appendable cppOutput;
    private final Appendable headerOutput;

    private NodeGenerator(
            CharStream input,
            Appendable cppOutput,
            Appendable headerOutput) {
        this.input = input;
        this.cppOutput = cppOutput;
        this.headerOutput = headerOutput;
    }

    public void generate() {
        GeneratorContext genCtx = ConfigParser.fromCharStream(
                input,
                DEFAULT_BASE_CLASS,
                "").parse(); // TODO: eliminate stem argument
        new CppEmitter(headerOutput, cppOutput, genCtx);
    }
}
