/**
 * Maintains state during configuration file parsing.
 *
 * Copyright (C) 2025 Eric Mintz
 * All Rights Reserved
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
package com.ooarchitect.visitingparsetree.nodegenerator.generator;

import com.google.common.collect.ImmutableSetMultimap;
import java.util.ArrayList;

/**
 * Accumulates node-related information during configuration file
 * parsing. The node emitter uses this information to emit C++
 * source that defines the nodes, their suppliers, and attributes
 * (if specified).
 */
public class GeneratorContext {

    public record NodeClassAndSuperclass(String supertype, String node)
            implements Comparable<NodeClassAndSuperclass> {

        @Override
        public int compareTo(NodeClassAndSuperclass that) {
            var result = supertype().compareTo(that.supertype());
            if (result == 0) {
                result = node().compareTo(that.node());
            }
            return result;
        }
    }

    private final String classHierarchyRoot;
    private final String hierarchyRootHeader;
    private final String declarationFilename;
    private final String implementationFilename;
    private final ArrayList<String> attributes;
    private final ArrayList<String> namespaces;
    private final ImmutableSetMultimap.Builder<String, NodeClassAndSuperclass> nodeDeclarationsBuilder;

    private String attributeClass;
    private String nodeClass;

    GeneratorContext(
            String classHierarchyRoot,
            String hierarchyRootHeader,
            String outputFileStem) {
        this.classHierarchyRoot = classHierarchyRoot;
        this.hierarchyRootHeader = hierarchyRootHeader;
        declarationFilename = outputFileStem + ".h";
        implementationFilename = outputFileStem + ".cpp";
        attributes = new ArrayList<>();
        namespaces = new ArrayList<>();
        nodeDeclarationsBuilder = ImmutableSetMultimap.builder();
        nodeClass = "";
    }

    void addAttribute(String attributeName) {
        attributes.add(attributeName);
    }

    void addNamespace(String namespace) {
        namespaces.add(namespace);
    }

    void addNode(String supertype, String subtype) {
        nodeDeclarationsBuilder.put(supertype, new NodeClassAndSuperclass(supertype, subtype));
    }

    public String attributeClass() {
        return attributeClass;
    }

    public ArrayList<String> attributes() {
        return new ArrayList<>(attributes);
    }

    public String classHierarchyRoot() {
        return classHierarchyRoot;
    }

    public String declarationFilename() {
        return declarationFilename;
    }

    public String implementationFilename() {
        return implementationFilename;
    }

    public String hierarchyRootHeader() {
        return hierarchyRootHeader;
    }

    public ArrayList<String> namespaces() {
        return new ArrayList<>(namespaces);
    }

    public String nodeClass() {
        return nodeClass;
    }

    public ImmutableSetMultimap<String, NodeClassAndSuperclass> nodeDeclarations() {
        return nodeDeclarationsBuilder.build();
    }

    public void setAttributeClass(String attributeClass) {
        this.attributeClass = attributeClass;
    }

    public void setNodeClass(String nodeClass) {
        this.nodeClass = nodeClass;
    }
}
