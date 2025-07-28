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
package org.ooarchitect.visitingparsetree.nodegenerator.generator;

import java.util.ArrayList;

/**
 * Accumulates node-related information during configuration file
 * parsing. The node emitter uses this information to emit C++
 * source that defines the nodes, their suppliers, and attributes
 * (if specified).
 */
public class GeneratorContext {

    public record NodeClassAndSuperclass(String supertype, String node) {
    }

    private final ArrayList<String> attributes;
    private final ArrayList<String> namespaces;
    private final ArrayList<NodeClassAndSuperclass> nodes;

    private String attributeClass;
    private String nodeClass;

    GeneratorContext() {
        attributes = new ArrayList<>();
        namespaces = new ArrayList<>();
        nodes = new ArrayList<>();
        nodeClass = "";
    }

    void addAttribute(String attributeName) {
        attributes.add(attributeName);
    }

    void addNamespace(String namespace) {
        namespaces.add(namespace);
    }

    void addNode(String supertype, String subtype) {
        nodes.add(new NodeClassAndSuperclass(supertype, subtype));
    }

    public String attributeClass() {
        return attributeClass;
    }

    public ArrayList<String> attributes() {
        return new ArrayList<>(attributes);
    }

    public ArrayList<String> namespaces() {
        return new ArrayList<>(namespaces);
    }

    public String nodeClass() {
        return nodeClass;
    }

    public ArrayList<NodeClassAndSuperclass> nodes() {
        return new ArrayList<>(nodes);
    }

    public void setAttributeClass(String attributeClass) {
        this.attributeClass = attributeClass;
    }

    public void setNodeClass(String nodeClass) {
        this.nodeClass = nodeClass;
    }
}
