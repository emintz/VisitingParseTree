/**
 * Validates the {@link ConfigParser} class
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

import com.google.common.collect.ImmutableSetMultimap;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

import org.ooarchitect.visitingparsetree.nodegenerator.generator.GeneratorContext.NodeClassAndSuperclass;

import static com.google.common.truth.Truth.assertThat;

@RunWith(JUnit4.class)
public class ConfigParserTest {

    private static final String DEFAULT_BASE_CLASS = "BaseAstNode";
    private static final String OUTPUT_FILE_STEM = "BaseAttrNode";
    private static final String SIMPLE_CONFIG =
            """
                    namespaces = { curly, larry, moe };
                    nodes = TestNodes { Foo, Bar, Fubb->Foo };
                    attributes = TestAttributes { silly, goofy, slapstick };
            """;

    @Test
    public void testSimpleConfig() {
        ConfigParser parser = ConfigParser.fromString(
                SIMPLE_CONFIG,
                DEFAULT_BASE_CLASS,
                OUTPUT_FILE_STEM);
        assertThat(parser).isNotNull();
        GeneratorContext context = parser.parse();
        assertThat(context).isNotNull();
        assertThat(context.namespaces()).containsExactly(
                "curly",
                "larry",
                "moe"
        );
        assertThat(context.nodeClass()).isEqualTo("TestNodes");
        ImmutableSetMultimap<String, NodeClassAndSuperclass> nodeDeclarations =
                context.nodeDeclarations();
        assertThat(nodeDeclarations).hasSize(3);
        assertThat(nodeDeclarations.get(DEFAULT_BASE_CLASS)).containsExactly(
                new NodeClassAndSuperclass(DEFAULT_BASE_CLASS, "Foo"),
                new NodeClassAndSuperclass(DEFAULT_BASE_CLASS, "Bar"));
        assertThat(nodeDeclarations.get("Foo")).containsExactly(
                new NodeClassAndSuperclass("Foo", "Fubb"));
        assertThat(context.attributeClass()).isEqualTo("TestAttributes");
        assertThat(context.attributes()).containsExactly(
                "silly",
                "goofy",
                "slapstick");
    }
}
