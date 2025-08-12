/**
 * Grammar for C++ attributed parse tree node generation
 *
 * <p>Copright 2025 Eric Mintz<br>
 *    All Rights Reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
grammar NodeGeneration;

/*
 * This is the generated code's entry point.
 */
configuration: configuration_body EOF;

configuration_body:
    namespace_declaration?
    node_declaration
    attribute_declaration?
    ;

namespace_declaration: NAMESPACES_KEYWORD EQUALS namespace_block SEMI;
namespace_block:  OPEN_BRACE namespace_list CLOSE_BRACE;
namespace_list: namespace_name (COMMA namespace_name)*;
namespace_name: ID;

node_declaration: NODES_KEYWORD EQUALS node_class_and_block SEMI;
node_class_and_block: node_class node_block;
node_class: ID;
node_block: OPEN_BRACE node_list CLOSE_BRACE;
node_list: node (COMMA node)*;
node: bare_node | qualified_node;
bare_node: ID;
qualified_node : node_subtype SUBTYPE_OF node_supertype;
node_supertype: ID;
node_subtype: ID;

attribute_declaration: ATTRIBUTES_KEYWORD EQUALS attribute_class_and_block SEMI;
attribute_class_and_block: attribute_class attribute_block;
attribute_class: ID;
attribute_block: OPEN_BRACE attribute_list CLOSE_BRACE;
attribute_list: attribute (COMMA attribute)*;
attribute: ID;


/* Keywords */
ATTRIBUTES_KEYWORD: 'attributes';
NAMESPACES_KEYWORD: 'namespaces';
NODES_KEYWORD: 'nodes';

/* Operators */
SUBTYPE_OF: '->';
CLOSE_BRACE: '}';
COMMA: ',';
EQUALS: '=';
OPEN_BRACE: '{';
QUOTE: '"';
SEMI: ';';

/* Identifiers and whitespace */
ID: ([A-Za-z]) ([A-Za-z] | '_')*;
WS: ( ' ' | '\t' | '\n' | '\r' ) + -> skip;
