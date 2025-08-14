/**
 * Validates {@link CppEmitter}
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

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

import java.io.IOException;

import static com.google.common.truth.Truth.assertThat;


@RunWith(JUnit4.class)
public class CppEmitterTest {

    private static final String EPOCH_START = "1970-01-01T00:00:00.000000000Z";
    private static final String TEST_USER = "ooarchitect.com";

    private static final String DEFAULT_BASE_CLASS = "VisitingParseTree::BaseAttrNode";

    private static final String TEST_CLASS_NAME = "Foo";
    private static final String OUTPUT_FILE_STEM = "BaseAttrNode";
    private static final String EXPECTED_HEADER_FILE = OUTPUT_FILE_STEM + ".h";

    private static final String SIMPLE_CONFIG =
            """
                    namespaces = { curly, larry, moe };
                    nodes = TestNodes { Foo, Bar, Fubb->Foo };
                    attributes = TestAttributes { silly, goofy, slapstick };
            """;

    private static final String EXPECTED_DECLARATION_FOOTER = """
            }
            }
            }
            
            #endif // TESTNODES_H_
            """;

    private static final String EXPECTED_FORWARD_DECLARATION = """
            class Foo;
            
            """;

    private static final String EXPECTED_DECLARATION_HEADER = """
            /**
             * Generated code, please DO NOT modify
             *
             * Generated on 1970-01-01T00:00:00.000000000Z by ooarchitect.com.
             */
            
            #ifndef TESTNODES_H_
            #define TESTNODES_H_
            
            #include <BaseAttrNode.h>
            
            #include <Attribute.h>
            #include <Supplier.h>
            #include <TraversalStatus.h>
            
            #include <memory>
            
            namespace curly {
            namespace larry {
            namespace moe {
            
            """;

    private static final String EXPECTED_IMPLEMENTATION_HEADER = """
            /**
             * Generated code, please DO NOT modify
             *
             * Generated on 1970-01-01T00:00:00.000000000Z by ooarchitect.com.
             */
            
            """
            + "#include \"" + EXPECTED_HEADER_FILE + "\"\n"
            + '\n'
            + """
            namespace curly {
            namespace larry {
            namespace moe {
            
            """;

    private static final String EXPECTED_ABSTRACT_NODE_IMPLEMENTATION = """
            Foo::Foo() :
                VisitingParseTree::BaseAttrNode() {}
            
            VisitingParseTree::TraversalStatus Foo::accept(VisitingParseTree::Visitor *visitor) {
              auto concrete_visitor = dynamic_cast<FooVisitor *>(visitor);
              return concrete_visitor
                  ? concrete_visitor->processFoo(this)
                  : VisitingParseTree::BaseAttrNode::accept(visitor);
            }
 
            """;

    private static final String EXPECTED_CONCRETE_NODE_IMPLEMENTATION =
       """
       Foo::Foo(forbid_public_access here) :
           VisitingParseTree::BaseAttrNode() {}
        
       VisitingParseTree::TraversalStatus Foo::accept(VisitingParseTree::Visitor *visitor) {
         auto concrete_visitor = dynamic_cast<FooVisitor *>(visitor);
         return concrete_visitor
             ? concrete_visitor->processFoo(this)
             : VisitingParseTree::BaseAttrNode::accept(visitor);
       }

       Foo::FooSupplier Foo::SUPPLIER;
                    
       VisitingParseTree::BaseAttrNodeSupplier& Foo::supplier(void) {
         return SUPPLIER;
       }
        
        """;

    private static final String EXPECTED_SUPPLIER_DECLARATION = """
              class FooSupplier : public VisitingParseTree::Supplier<VisitingParseTree::BaseAttrNode> {
                friend class Foo;
                FooSupplier(void);
              public:
                virtual ~FooSupplier() = default;
                virtual std::shared_ptr<VisitingParseTree::BaseAttrNode> make_shared(void) override;
              };
            """;

    private static final String EXPECTED_ABSTRACT_NODE_DECLARATION = """
            class Foo : public VisitingParseTree::BaseAttrNode {
            protected:
              Foo(void);
            
            public:
            
              virtual VisitingParseTree::TraversalStatus accept(VisitingParseTree::Visitor *visitor) override;
              
              virtual ~Foo() = default;
            };
            
            """;

    private static final String EXPECTED_CONCRETE_NODE_DECLARATION = """
            class Foo : public VisitingParseTree::BaseAttrNode {
              friend class FooSupplier;
            public:
              Foo(forbid_public_access here);
            
            """
            + EXPECTED_SUPPLIER_DECLARATION
            + """
            
              static FooSupplier SUPPLIER;
            
              virtual VisitingParseTree::BaseAttrNodeSupplier& supplier(void) override;
            
              virtual VisitingParseTree::TraversalStatus accept(VisitingParseTree::Visitor *visitor) override;
            
              virtual ~Foo() = default;
            };
            
            """;

    private static final String EXPECTED_SUPPLIER_IMPLEMENTATION = """
            Foo::FooSupplier::FooSupplier(void) :
              VisitingParseTree::Supplier<VisitingParseTree::BaseAttrNode>("curly::larry::moe::Foo") {}
            
            std::shared_ptr<VisitingParseTree::BaseAttrNode> Foo::FooSupplier::make_shared(void) {
              return std::static_pointer_cast<VisitingParseTree::BaseAttrNode>(
                std::make_shared<Foo>(forbid_public_access::here));
            }
            """;

    // was:
    // std::shared_ptr<VisitingParseTree::BaseAttrNode> Foo::FooSupplier::make_shared(void) {
    //              Foo *node = new Foo(forbid_public_access::here);
    //              return std::shared_ptr<VisitingParseTree::BaseAttrNode>(node);
    //            }

    private static final String EXPECTED_VISITOR_DECLARATION = """
            class FooVisitor {
            public:
              virtual ~FooVisitor() = default;
              virtual VisitingParseTree::TraversalStatus processFoo(Foo *host) = 0;
            };
            """;

    private CppEmitter emitter;
    private GeneratorContext context;
    private StringBuilder declarationTarget;
    private StringBuilder implementationTarget;

    @Before
    public void setUp() {
        ConfigParser parser = ConfigParser.fromString(
                SIMPLE_CONFIG,
                DEFAULT_BASE_CLASS, OUTPUT_FILE_STEM);
        context = parser.parse();
        declarationTarget = new StringBuilder();
        implementationTarget = new StringBuilder();
        emitter = new CppEmitter(
                declarationTarget,
                implementationTarget,
                context,
                () -> EPOCH_START,
                () -> TEST_USER);
    }

    @Test
    public void testSomething() throws IOException {
        emitter.emit();
        System.out.println("Generated header:\n");
        System.out.println(declarationTarget.toString());
        System.out.println();
        System.out.println("Generated implementation:\n");
        System.out.println(implementationTarget.toString());
    }

    @Test
    public void testDeclarationFooter() throws IOException {
        emitter.declarationFooter();
        assertThat(declarationTarget.toString()).isEqualTo(EXPECTED_DECLARATION_FOOTER);
    }

    @Test
    public void testForwardDeclaration() throws IOException {
        emitter.forwardDeclaration(TEST_CLASS_NAME);
        assertThat(declarationTarget.toString()).isEqualTo(EXPECTED_FORWARD_DECLARATION);
        assertThat(implementationTarget.toString()).isEmpty();
    }

    @Test
    public void testDeclarationHeader() throws IOException {
        emitter.declarationHeader();
        assertThat(declarationTarget.toString()).isEqualTo(EXPECTED_DECLARATION_HEADER);
        assertThat(implementationTarget.toString()).isEmpty();
    }

    @Test
    public void testImplementationHeader() throws IOException {
        emitter.implementationHeader();
        assertThat(declarationTarget.toString()).isEmpty();
        assertThat(implementationTarget.toString())
                .isEqualTo(EXPECTED_IMPLEMENTATION_HEADER);
    }

    @Test
    public void testSupplierDeclaration() throws IOException {
        emitter.supplierDeclaration("Foo");
        assertThat(declarationTarget.toString()).isEqualTo(EXPECTED_SUPPLIER_DECLARATION);
        assertThat(implementationTarget.toString()).isEmpty();
    }

    @Test
    public void testSupplierImplementation() throws IOException {
        emitter.supplierImplementation("Foo");
        assertThat(declarationTarget.toString()).isEmpty();
        assertThat(implementationTarget.toString())
                .isEqualTo(EXPECTED_SUPPLIER_IMPLEMENTATION);
    }

    @Test
    public void testVisitorDeclaration() throws IOException {
        emitter.visitorDeclaration("Foo");
        assertThat(declarationTarget.toString()).isEqualTo(EXPECTED_VISITOR_DECLARATION);
        assertThat(implementationTarget.toString()).isEmpty();
    }

    @Test
    public void testAbstractNodeDeclaration() throws IOException {
        emitter.abstractNodeDeclaration("Foo", DEFAULT_BASE_CLASS);
        assertThat(declarationTarget.toString()).isEqualTo(EXPECTED_ABSTRACT_NODE_DECLARATION);
        assertThat(implementationTarget.toString()).isEmpty();
    }

    @Test
    public void testConcreteNodeDeclaration() throws IOException {
        emitter.concreteNodeDeclaration("Foo", DEFAULT_BASE_CLASS);
        assertThat(declarationTarget.toString())
                .isEqualTo(EXPECTED_CONCRETE_NODE_DECLARATION);
        assertThat(implementationTarget.toString()).isEmpty();
    }

    @Test
    public void testAbstractNodeImplementation() throws IOException {
        emitter.abstractNodeImplementation("Foo", DEFAULT_BASE_CLASS);
        assertThat(declarationTarget.toString()).isEmpty();
        assertThat(implementationTarget.toString()).isEqualTo(EXPECTED_ABSTRACT_NODE_IMPLEMENTATION);
    }

    @Test
    public void testConcreteNodeImplementation() throws IOException {
        emitter.concreteNodeImplementation("Foo", DEFAULT_BASE_CLASS);
        assertThat(declarationTarget.toString()).isEmpty();
        assertThat(implementationTarget.toString()).isEqualTo(EXPECTED_CONCRETE_NODE_IMPLEMENTATION);
    }
}