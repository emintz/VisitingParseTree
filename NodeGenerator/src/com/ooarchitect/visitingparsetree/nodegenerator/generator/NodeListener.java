/**
 * Listener that accumulates the node generation configuration.
 * The gathered configuration information will be passed to
 * the emitter.
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

import com.ooarchitect.visitingparsetree.nodegenerator.grammar.NodeGenerationBaseListener;
import com.ooarchitect.visitingparsetree.nodegenerator.grammar.NodeGenerationParser;


public class NodeListener extends NodeGenerationBaseListener {

    private final GeneratorContext context;
    private final String defaultBaseNodeClassClass;

    private String nodeBaseClass;
    private String nodeClass;

    NodeListener(
            GeneratorContext context,
            String defaultBaseNodeClass) {
        this.context = context;
        this.defaultBaseNodeClassClass = defaultBaseNodeClass;
        nodeBaseClass = "";
        nodeClass = "";
    }

    @Override
    public void enterNode(NodeGenerationParser.NodeContext ctx) {
        nodeBaseClass = defaultBaseNodeClassClass;
        nodeClass = "";
    }

    @Override
    public void exitAttribute(
            NodeGenerationParser.AttributeContext ctx) {
        context.addAttribute(ctx.getText());
    }

    @Override
    public void exitAttribute_class(
            NodeGenerationParser.Attribute_classContext ctx) {
        context.setAttributeClass(ctx.getText());
    }

    @Override
    public void exitBare_node(
            NodeGenerationParser.Bare_nodeContext ctx) {
        nodeClass = ctx.getText();
    }

    @Override
    public void exitNamespace_name(
            NodeGenerationParser.Namespace_nameContext ctx) {
        String namespaceName = ctx.getText();
        context.addNamespace(namespaceName);
    }

    @Override
    public void exitNode(
            NodeGenerationParser.NodeContext ctx) {
        context.addNode(nodeBaseClass, nodeClass);
    }

    @Override
    public void exitNode_class(
            NodeGenerationParser.Node_classContext ctx) {
        context.setNodeClass(ctx.getText());
    }

    @Override
    public void exitNode_subtype(
            NodeGenerationParser.Node_subtypeContext ctx) {
        nodeClass = ctx.getText();
    }

    @Override
    public void exitNode_supertype(
            NodeGenerationParser.Node_supertypeContext ctx) {
        nodeBaseClass = ctx.getText();
    }
}
