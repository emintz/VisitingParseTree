/**
 * The ANTLR-4-based parser that analyzes the node configuration
 * file.
 * <p>
 * Copyright (C) 2025 Eric Mintz
 * All Rights Reserved
 * <p>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * <p>
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * <p>
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
package org.ooarchitect.visitingparsetree.nodegenerator.generator;

import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.CharStreams;
import org.antlr.v4.runtime.CommonTokenStream;
import org.ooarchitect.visitingparsetree.nodegenerator.grammar.NodeGenerationLexer;
import org.ooarchitect.visitingparsetree.nodegenerator.grammar.NodeGenerationParser;

import java.io.IOException;
import java.nio.file.Path;

/**
 * Wrapper around the generated node configuration file parser. This
 * is a convenience class for use by the code generator. The
 * ANTLR-generated classes SHOULD NOT be access directly.
 */
public class ConfigParser {
    GeneratorContext generatorContext = new GeneratorContext();
    NodeGenerationParser parser;

    /**
     * Constructor
     *
     * @param configuration {@link CharStream} providing the coonfiguration
     */
    private ConfigParser(
            CharStream configuration,
            String defaulBaseNodeClass) {
        NodeGenerationLexer lexer =
                new NodeGenerationLexer(configuration);
        // TODO: error handling
        CommonTokenStream tokenStream = new CommonTokenStream(lexer);
        generatorContext = new GeneratorContext();
        NodeListener listener = new NodeListener(
                generatorContext, defaulBaseNodeClass);
        parser = new NodeGenerationParser(tokenStream);
        parser.addParseListener(listener);
        // TODO: error handling
    }

    /**
     * Runs the parser
     *
     * @return a {@link GeneratorContext} containing the parsed
     *         node configuration.
     */
    GeneratorContext parse() {
        parser.configuration();
        return generatorContext;
    }

    /**
     * Builds a parser that takes its input from the specified {@link String}
     *
     * @param input node configuration encoded in UTF-8
     * @param defaultBaseNodeClass the root for the generated
     *                             node class hierarchy. The
     *                             generator bases all unqualified
     *                             nodes on this class.
     *
     * @return a {@code ConfigParser} that will parse the provided
     *         configuration.
     */
    public static ConfigParser fromString(
            String input,
            String defaultBaseNodeClass) {
        System.out.println("Creating configuration parser from string:");
        System.out.println(input);
        return new ConfigParser(
                CharStreams.fromString(input),
                defaultBaseNodeClass);
    }

    /**
     * Builds a parser that takes its input from the specified file
     *
     * @param fileName name of a file containing a node configuration
     *                 that is encoded in UTF-8
     * @param defaultBaseNodeClass the root for the generated
     *                             node class hierarchy. The
     *                             generator bases all unqualified
     *                             nodes on this class.
     * @return a {@code ConfigParser} that will parse the contents
     *         of the specified file.
     * @throws IOException if the file does not exist or cannot
     *                     be opened
     */
    public static ConfigParser fromFileName(
            String fileName,
            String defaultBaseNodeClass)
            throws IOException {
        return new ConfigParser(
                CharStreams.fromFileName(fileName),
                defaultBaseNodeClass);
    }

    /**
     * Builds a parser that takes its input from the specified file
     *
     * @param filePath qualified path of a file containing a node
     *                 configuration that is encoded in UTF-8
     * @param defaultBaseNodeClass the root for the generated
     *                             node class hierarchy. The
     *                             generator bases all unqualified
     *                             nodes on this class.
     * @return a {@code ConfigParser} that will parse the contents
     *         of the specified file.
     * @throws IOException if the file does not exist or cannot
     *                     be opened
     */
    public static ConfigParser fromPath(
            Path filePath,
            String defaultBaseNodeClass)
            throws IOException {
        return new ConfigParser(
                CharStreams.fromPath(filePath),
                defaultBaseNodeClass);
    }
}
