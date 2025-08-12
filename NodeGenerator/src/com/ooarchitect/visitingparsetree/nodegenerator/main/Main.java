/**
 * Entry point for node generation
 *
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

package com.ooarchitect.visitingparsetree.nodegenerator.main;

import org.apache.commons.cli.*;
import com.ooarchitect.visitingparsetree.nodegenerator.generator.NodeGenerator;

import java.io.IOException;
import java.util.Optional;

public class Main {

    private static Optional<CommandLine> parseArguments(String[] args) {
        Optional<CommandLine> parsedCommandLine = Optional.empty();
        System.out.print("Command line args:");
        for (String arg : args) {
            System.out.print(" " + arg);
        }
        System.out.println();
        Option inputFile = Option.builder("f")
                .argName("input file")
                .longOpt("file")
                .hasArg()
                .type(String.class)
                .valueSeparator()
                .desc("File containing node declarations")
                .build();
        Option help = Option.builder("h")
                .argName("Print help message")
                .desc("Print this message and exit")
                .longOpt("help")
                .optionalArg(true)
                .build();
        Option outputFile = Option.builder("o")
                .argName("output file")
                .longOpt("output")
                .type(String.class)
                .valueSeparator()
                .desc("Output file, writes to stdout if omitted")
                .optionalArg(true)
                .build();
        Options commandLineOptions = new Options()
                .addOption(inputFile)
                .addOption(help)
                .addOption(outputFile);
        HelpFormatter helpFormatter = new HelpFormatter();
        try {
            CommandLineParser parser = new DefaultParser();
            CommandLine cmd = parser.parse(commandLineOptions, args);
            if (cmd.hasOption('h') || !cmd.hasOption('f')) {
                helpFormatter.printHelp("generator ", commandLineOptions, true);
            } else {
                parsedCommandLine = Optional.of(cmd);
            }
        } catch (ParseException e) {
            System.err.print("Invalid command line: ");
            System.err.println(e.getMessage());
        }
        return parsedCommandLine;
    }

    public static void main(String[] args) throws IOException {
        Optional<CommandLine> maybeParsedCommandLine = parseArguments(args);
        if (maybeParsedCommandLine.isPresent()) {
            var parsedCommandLine = maybeParsedCommandLine.get();
            NodeGenerator.Builder generatorBuilder = NodeGenerator.builder();
            if (parsedCommandLine.hasOption("o")) {
                generatorBuilder.outputToFile(parsedCommandLine.getOptionValue("o"));
            } else {
                generatorBuilder.outputToStdout();
            }
            generatorBuilder.inputFromFile(parsedCommandLine.getOptionValue("f"));
            generatorBuilder.build().generate();
        }
    }
}