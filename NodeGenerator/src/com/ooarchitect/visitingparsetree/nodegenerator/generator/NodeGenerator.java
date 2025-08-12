package com.ooarchitect.visitingparsetree.nodegenerator.generator;

import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.CharStreams;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.nio.file.Path;

public class NodeGenerator {
    public static class Builder {
        private CharStream input;
        private AutoCloseableAppendable cppOutput;
        private AutoCloseableAppendable headerOutput;
        private String fileStem;

        private Builder() {
            input = null;
            cppOutput = null;
            headerOutput = null;
            fileStem = null;
        }

        public Builder generatedFileStem(String fileStem) {
            this.fileStem = fileStem;
            return this;
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
            cppOutput = AppendableWrappers.ofWriter(new OutputStreamWriter(cppStream));
            FileOutputStream headerStream = new FileOutputStream(fileName + ".h");
            headerOutput = AppendableWrappers.ofWriter(new OutputStreamWriter(headerStream));
            fileStem = Path.of(fileName).getFileName().toString();
            return this;
        }

        public Builder outputToStdout() {
            AutoCloseableAppendable dontCloseStdout = AppendableWrappers.ofWriter(System.out);
            cppOutput = dontCloseStdout;
            headerOutput = dontCloseStdout;
            return this;
        }

        public Builder outputToString(StringBuilder output) {
            AutoCloseableAppendable wrapper = AppendableWrappers.ofAppendable(output);
            this.cppOutput = wrapper;
            this.headerOutput = wrapper;
            return this;
        }

        public NodeGenerator build() {
            return new NodeGenerator(input, cppOutput, headerOutput, fileStem);
        }
    }

    public static Builder builder() {
        // TODO: validate settings.
        return new Builder();
    }

    private static final String DEFAULT_BASE_CLASS =
            "VisitingParseTree::BaseAttrNode";

    private final String fileStem;
    private final CharStream input;
    private final AutoCloseableAppendable cppOutput;
    private final AutoCloseableAppendable headerOutput;

    private NodeGenerator(
            CharStream input,
            AutoCloseableAppendable cppOutput,
            AutoCloseableAppendable headerOutput,
            String fileStem) {
        this.input = input;
        this.cppOutput = cppOutput;
        this.headerOutput = headerOutput;
        this.fileStem = fileStem;
    }

    public void generate() throws IOException {
        GeneratorContext genCtx = ConfigParser.fromCharStream(
                input,
                DEFAULT_BASE_CLASS,
                fileStem).parse();
        try (headerOutput; cppOutput) {
            new CppEmitter(headerOutput, cppOutput, genCtx)
                    .emit();
        } catch (Exception e) {  // TODO: WTF? Fix me!
            throw new RuntimeException(e);
        }
    }
}
