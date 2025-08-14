/**
 * Emits C++ node definitions.
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

import com.google.common.annotations.VisibleForTesting;

import java.io.IOException;
import java.time.Instant;
import java.util.ArrayDeque;
import java.util.HashSet;
import java.util.List;
import java.util.function.Supplier;

public class CppEmitter {
    private static final Supplier<String> CURRENT_TIME =
            () -> Instant.now().toString();
    private static final Supplier<String> USER_NAME =
            () -> System.getProperty("user.name");
    private static final String TRAVERSAL_STATUS =
                "VisitingParseTree::TraversalStatus";
    private static final String BASE_VISITOR_CLASS =
            "VisitingParseTree::Visitor";

    private final Appendable declarationTarget;
    private final Appendable implementationTarget;
    private final GeneratorContext context;
    private final Supplier<String> currentTime;
    private final Supplier<String> userName;

    private final String classHierarchyRoot;
    private final String baseSupplierClass;

    /**
     * Returns the visitor class associated with the provided node class
     * @param nodeClassName node class associated with the visitor
     * @return the visitor class name, as above
     */
    private static String toVisitorClassName(String nodeClassName) {
        return nodeClassName + "Visitor";
    }

    /**
     * Returns the visitation method for the visitor associated with the
     * specified node class
     * @param nodeClassName the node class, as above
     * @return the method name, as above
     */
    private static String toProcessMethod(String nodeClassName) {
        return "process" + nodeClassName;
    }

    /**
     * Returns the supplier class for the specified node class
     * @param nodeClassName node class name
     * @return supplier class name that supplies the specified node
     */
    private static String toSupplierName(String nodeClassName) {
        return nodeClassName + "Supplier";
    }

    /**
     * Return the prefix for members of the specified node class
     * @param nodeClassName the node class name
     * @return the prefix
     */
    private static String withinClass(String nodeClassName) {
        return nodeClassName + "::";
    }

    /**
     * Returns the fully qualified prefix for members of the specified
     * node's supplier
     * @param nodeClassName the node class name
     * @return the prefix
     */
    private static String withinSupplier(String nodeClassName) {
        return withinClass(nodeClassName) + toSupplierName(nodeClassName) + "::";
    }

    private void closeNamespaces(Appendable target) throws IOException {
        for (String namespace : context.namespaces()) {
            target.append("}\n");
        }
    }

    private void openNamespaces(Appendable target) throws IOException {
        if (!context.namespaces().isEmpty()) {
            for (String namespace : context.namespaces()) {
                target.append("namespace ")
                        .append(namespace)
                        .append(" {\n");
            }
            target.append('\n');
        }
    }

    /**
     * Emit the boilerplate comments at the beginning of the file.
     *
     * @param target Appendable that receives the generated header
     * @return target, for chaining
     * @throws IOException if appending failed
     */
    private Appendable fileHeader(Appendable target) throws IOException {
        target.append("/**\n")
                .append(" * Generated code, please DO NOT modify\n")
                .append(" *\n")
                .append(" * Generated on ")
                    .append(currentTime.get())
                    .append(" by ")
                    .append(userName.get())
                    .append(".\n");
                if (context.hasInputFile()) {
                    target.append(" * Source: ").append(context.inputFileName()).append('\n');
                }
                target.append(" */\n")
                .append('\n');

        return target;
    }

    /**
     * Declare attributes if the configuration specifies any. Does
     * nothing if the user does not request attributes.
     *
     * @throws IOException on error
     */
    void attributeDeclaration() throws IOException {
        List<String> attributes = context.attributes();
        if (!attributes.isEmpty()) {
            String attributeClass = context.attributeClass();
            declarationTarget
                    .append('\n')
                    .append("class ")
                        .append(attributeClass)
                    .append(" : ")
                    .append("VisitingParseTree::Attribute")
                        .append(" {\n")
                    .append("  ")
                        .append(attributeClass)
                        .append("(const char *name);\n")
                    .append("public:\n");
            for (String name : context.attributes()) {
                declarationTarget
                        .append("  static ")
                            .append(attributeClass)
                            .append(' ')
                            .append(name)
                            .append(";\n");
            }
            declarationTarget.append("};\n");
        }
    }

    void attributeImplementation() throws IOException {
        String attributeClass = context.attributeClass();
        if (!context.attributes().isEmpty()) {
            String attributeNamePrefix = new StringBuilder(context.fullyQualifiedPrefixFrom(attributeClass))
                    .append("::")
                    .toString();
            implementationTarget
                    .append(attributeClass)
                    .append("::")
                    .append(attributeClass)
                    .append("(const char *name) :\n")
                    .append("    VisitingParseTree::Attribute(name) {}\n");
            for (String name: context.attributes()) {
                implementationTarget
                        .append('\n')
                        .append(attributeClass)
                            .append(' ')
                            .append(attributeClass)
                            .append("::")
                            .append(name)
                            .append("(\"")
                            .append(attributeNamePrefix)
                            .append(name)
                            .append("\");\n");
            }
        }
    }

    /**
     * Emits the source file header, including introductory
     * comments, include guard, and includes.
     *
     * @throws IOException when appending generated text fails
     */
    @VisibleForTesting
    void declarationHeader() throws IOException {
        String guard = context.nodeClass().toUpperCase() + "_H_";
        fileHeader(declarationTarget)
                .append("#ifndef ").append(guard).append('\n')
                .append("#define ").append(guard).append('\n')
                .append('\n')
                .append("#include <")
                    .append(context.hierarchyRootHeader())
                    .append(">\n")
                .append("\n")
                .append("#include <Attribute.h>\n")
                .append("#include <Supplier.h>\n")
                .append("#include <TraversalStatus.h>\n")
                .append('\n')
                .append("#include <memory>\n")
                .append('\n');
        openNamespaces(declarationTarget);
    }

    /**
     * Emits the header file footer, including the include guard
     * closure.
     *
     * @throws IOException when appending generated text fails
     */
    @VisibleForTesting
    void declarationFooter() throws IOException {
        closeNamespaces(declarationTarget);
        String guard = context.nodeClass().toUpperCase() + "_H_";
        declarationTarget.append('\n')
                .append("#endif // ").append(guard).append('\n');
    }

    void implementationFooter() throws IOException {
        closeNamespaces(implementationTarget);
    }

    @VisibleForTesting
    Appendable implementationHeader() throws IOException {
        fileHeader(implementationTarget)
                .append("#include \"").append(context.declarationFilename()).append("\"\n")
                .append('\n');
        openNamespaces(implementationTarget);
        return implementationTarget;
    }

    /**
     * Emits a forward class declaration at the outermost scope
     *
     * @param className class to declare
     * @throws IOException when appending generated text fails
     */
    @VisibleForTesting
    void forwardDeclaration(String className)
        throws IOException {
        declarationTarget.append("class ").append(className).append(";\n\n");
    }

    /**
     * Emits the preamble boilerplate required for abstract and
     * concrete node class declarations.
     *
     * @param nodeClassName emitted node's class name
     * @param superclassName emitted node supertype
     * @return the {@link Appendable} that receives the node
     *         declaration for chaining
     * @throws IOException when appending generated text fails
     */
    private Appendable nodeClassPreamble(
            String nodeClassName,
            String superclassName)
        throws IOException {
        return declarationTarget
                .append("class ")
                .append(nodeClassName)
                .append(" : public ").append(superclassName).append(" {\n")
                ;
    }

    /**
     * Closes a node class declaration by emitting the end of class
     * boilerplate.
     * @param nodeClassName class being close
     * @throws IOException when appending generated text fails
     */
    private void nodeDeclarationClose(
            String nodeClassName) throws IOException {
        declarationTarget
                .append("  virtual " + TRAVERSAL_STATUS + " accept(" + BASE_VISITOR_CLASS + " *visitor) override;\n")
                .append('\n')
                .append("  virtual ~").append(nodeClassName).append("() = default;\n")
                .append("};\n")
                .append('\n');
    }

    /**
     * Emits an abstract node class implementation (.cpp)
     * @param nodeClassName class to implement
     * @param superclassName immediate superclass of the class being
     *                       emitted
     * @throws IOException when appending generated text fails
     */
    @VisibleForTesting
    void abstractNodeImplementation(
            String nodeClassName,
            String superclassName)
            throws IOException {
        nodeImplementation(nodeClassName, superclassName, "");
    }

    /**
     * Generates an abstract node's declaration
     * @param nodeClassName generated node's class name
     * @param superclassName generated node's immediate superclass
     * @throws IOException when appending generated text fails
     */
    @VisibleForTesting
    void abstractNodeDeclaration(
            String nodeClassName,
            String superclassName)
            throws IOException {
        nodeClassPreamble(nodeClassName, superclassName)
                .append("protected:\n")
                .append("  ").append(nodeClassName).append("(void);\n")
                .append('\n')
                .append("public:\n")
                .append('\n');
        nodeDeclarationClose(nodeClassName);
    }

    /**
     * Generates a concrete node's declaration
     * @param nodeClassName generated node's class name
     * @param superclassName generated node's immediate superclass
     * @throws IOException when appending generated text fails
     */
    @VisibleForTesting
    void concreteNodeDeclaration(
            String nodeClassName,
            String superclassName)
            throws IOException {
        String supplierName = toSupplierName(nodeClassName);
        nodeClassPreamble(nodeClassName, superclassName)
                .append("  friend class ").append(supplierName).append(";\n")
                .append("public:\n")
                .append("  ").append(nodeClassName).append("(forbid_public_access here);\n")
                .append('\n');
        supplierDeclaration(nodeClassName)
                .append('\n')
                .append("  static ").append(supplierName).append(" SUPPLIER;\n")
                .append('\n')
                .append("  virtual ").append(baseSupplierClass).append("& supplier(void) override;\n")
                .append('\n');
        nodeDeclarationClose(nodeClassName);
    }

    /**
     * Generates a concrete node's implementation
     * @param nodeClassName generated node's class name
     * @param superclassName generated node's immediate superclass
     * @throws IOException when appending generated text fails
     */
    @VisibleForTesting
    void concreteNodeImplementation(
            String nodeClassName,
            String superclassName) throws IOException{
        String withinClass = nodeClassName + "::";
        nodeImplementation(
                nodeClassName,
                superclassName,
                "forbid_public_access here")
                .append(withinClass)
                    .append(toSupplierName(nodeClassName))
                    .append(' ')
                    .append(withinClass)
                    .append("SUPPLIER;\n")
                .append('\n')
                .append(baseSupplierClass).append("& ").append(withinClass).append("supplier(void) {\n")
                .append("  return SUPPLIER;\n")
                .append("}\n")
                .append('\n');
    }

    /**
     * Generates the methods and field implementation used in abstract and
     * concrete nodes.
     *
     * @param nodeClassName        generated node's class name
     * @param superclassName       generated node's immediate superclass
     * @param constructorArguments
     * @return the {@link Appendable} to receive additional emitted
     * text for chaining
     * @throws IOException when appending generated text fails
     */
    private Appendable nodeImplementation(
            String nodeClassName,
            String superclassName,
            String constructorArguments) throws IOException {
        String withinClass = nodeClassName + "::";
        return implementationTarget
                .append(withinClass)
                    .append(nodeClassName)
                    .append("(")
                    .append(constructorArguments)
                    .append(") :\n")
                .append("    ")
                    .append(superclassName)
                    .append("() {}\n")
                .append('\n')
                .append(TRAVERSAL_STATUS + ' ').append(withinClass).append("accept(" + BASE_VISITOR_CLASS + " *visitor) {\n")
                .append("  auto concrete_visitor = dynamic_cast<").append(toVisitorClassName(nodeClassName)).append(" *>(visitor);\n")
                .append("  return concrete_visitor\n")
                .append("      ? concrete_visitor->").append(toProcessMethod(nodeClassName)).append("(this)\n")
                .append("      : ").append(superclassName).append("::accept(visitor);\n")
                .append("}\n")
                .append('\n')
        ;
    }

    /**
     * Generates a node supplier declaration
     * @param nodeClassName generated node's class name
     * @return the {@link Appendable} to receive additional emitted
     *         text for chaining
     * @throws IOException when appending generated text fails
     */
    @VisibleForTesting
    Appendable supplierDeclaration(String nodeClassName)
            throws IOException {
        String supplierClassName = nodeClassName + "Supplier";
        return declarationTarget
                .append("  class ")
                .append(supplierClassName)
                .append(" : public VisitingParseTree::Supplier<")
                .append(classHierarchyRoot)
                .append("> {\n")
                .append("    friend class ")
                    .append(nodeClassName)
                    .append(";\n")
                .append("    ").append(supplierClassName).append("(void);\n")
                .append("  public:\n")
                .append("    virtual ~").append(supplierClassName).append("() = default;\n")
                .append("    virtual std::shared_ptr<")
                    .append(classHierarchyRoot)
                    .append("> make_shared(void) override;\n")
                .append("  };\n")
        ;
    }

    /**
     * Generates a node supplier implementation
     * @param nodeClassName generated node's class name
     * @throws IOException when appending generated text fails
     */
    @VisibleForTesting
    void supplierImplementation(String nodeClassName)
                throws IOException {
        String supplierClassName = toSupplierName(nodeClassName);
        String withinSupplier = withinSupplier(nodeClassName);
        String supplierNamePrefix = context.fullyQualifiedPrefixFrom("");

        implementationTarget
                .append(withinSupplier).append(supplierClassName).append("(void) :\n")
                .append("  VisitingParseTree::Supplier<")
                    .append(classHierarchyRoot)
                    .append(">(\"")
                    .append(supplierNamePrefix)
                    .append(nodeClassName)
                    .append("\") {}\n")
                .append('\n')
              .append("std::shared_ptr<")
                    .append(classHierarchyRoot)
                    .append("> ")
                    .append(withinSupplier)
                    .append("make_shared(void) {\n")
                .append("  return std::static_pointer_cast<")
                    .append(classHierarchyRoot)
                    .append(">(\n")
                .append("    std::make_shared<")
                    .append(nodeClassName)
                    .append(">(forbid_public_access::here));\n")
                .append("}\n");
    }

    /**
     * Generates the visitor class associated with the specified node
     * @param nodeClassName generated node's class name
     * @throws IOException when appending generated text fails
     */
    @VisibleForTesting
    void visitorDeclaration(String nodeClassName)
            throws IOException {
        String visitorClassName = toVisitorClassName(nodeClassName);
        declarationTarget
                .append("class ").append(visitorClassName).append(" {\n")
                .append("public:\n")
                .append("  virtual ~").append(visitorClassName).append("() = default;\n")
                .append("  virtual " + TRAVERSAL_STATUS + " process")
                    .append(nodeClassName)
                    .append('(')
                    .append(nodeClassName)
                    .append(" *host) = 0;\n")
                .append("};\n");
    }


    /**
     * Constructor for use by applications. The resulting
     * instance uses the current time and username.
     *
     * @param declarationTarget receives generated class
     *                          declarations (.h)
     * @param implementationTarget receives generated class
     *                             implementations (.cpp)
     */
    CppEmitter(
            Appendable declarationTarget,
            Appendable implementationTarget,
            GeneratorContext context) {
        this.declarationTarget = declarationTarget;
        this.implementationTarget = implementationTarget;
        this.context = context;
        this.currentTime = CURRENT_TIME;
        this.userName = USER_NAME;
        this.classHierarchyRoot = context.classHierarchyRoot();
        this.baseSupplierClass = "VisitingParseTree::Supplier<VisitingParseTree::BaseAttrNode>" ;
    }

    /**
     * Constructor for use by tests. The constructed instance
     * uses the time and username provided by the test.
     *
     * @param declarationTarget receives the generated code
     * @param context provides the class configurations
     * @param timeSupplier      supplies the test time
     * @param userNameSupplier  supplies the test username
     */
    @VisibleForTesting
    CppEmitter(
            Appendable declarationTarget,
            Appendable implementationTarget,
            GeneratorContext context,
            Supplier<String> timeSupplier,
            Supplier<String> userNameSupplier) {
        this.declarationTarget = declarationTarget;
        this.implementationTarget = implementationTarget;
        this.currentTime = timeSupplier;
        this.userName = userNameSupplier;
        this.context = context;
        this.classHierarchyRoot = context.classHierarchyRoot();
        this.baseSupplierClass = this.classHierarchyRoot + "Supplier";
    }

    void nodes() throws IOException {
        HashSet<String> emitted = new HashSet<>();
        ArrayDeque<String> pending = new ArrayDeque<>();
        pending.addLast(context.classHierarchyRoot());
        var nodeDeclarations = context.nodeDeclarations();
        while (!pending.isEmpty()) {
            String currentSuperclass = pending.removeFirst();
            for (var currentNodeAndSuperclass : nodeDeclarations.get(currentSuperclass)) {
                var currentNode = currentNodeAndSuperclass.node();
                if (emitted.contains(currentNode)) {
                    throw new GeneratorException(
                            "Node " + currentNode
                            + " is multiply defined or in an inheritance loop.");
                }
                if (nodeDeclarations.containsKey(currentNode)) {
                    abstractNodeDeclaration(currentNode, currentSuperclass);
                    abstractNodeImplementation(currentNode, currentSuperclass);
                    pending.addLast(currentNode);
                } else {
                    concreteNodeDeclaration(currentNode, currentSuperclass);
                    concreteNodeImplementation(currentNode, currentSuperclass);
                    supplierImplementation(currentNode);
                }
                visitorDeclaration(currentNode);
                /*
                 * Note: visitor classes use default constructors and
                 *       destructors. Since their only other method is
                 *       pure virtual, they have no implementation.
                 */
                emitted.add(currentNode);
            }
        }
    }

    /**
     * Emits the classes specified by the provided context
     *
     * @throws IOException when appending generated text fails
     */
    void emit() throws IOException {
        declarationHeader();
        implementationHeader();

        nodes();
        attributeDeclaration();
        attributeImplementation();

        declarationFooter();
        implementationFooter();

    }
}
