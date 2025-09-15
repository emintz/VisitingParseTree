# Visiting Parse Tree Library

The Visiting Parse Tree library supports N-ary syntax trees, and
provides:

* Templated base classes that implement:
  * Basic tree toplology and depth-first in order tree traversal
  * 'dynamic_cast'-based generalized double dispatch <!-- TODO: link -->
  * Double dispatch and arbitrary 
    [`std::string`](https://en.cppreference.com/w/cpp/string/basic_string.html)-valued
    attributes <!-- TODO: link -->
* 'BaseAttrNode', a (non-templated) base node class supporting attributes and
  visitation
* A code generator that produces abstract and concrete attributed nodes
  that inherit `BaseAttrNode`.

## Base Node Templates

The base node templates are the heart of the library. Since they are
designed for inheritance, their constructors are `protected` to prevent
direct instiation. They implement compile-time polymorphism via the
Curiously Recurring Template Pattern 
[CppReference](https://en.cppreference.com/w/cpp/language/crtp.html)
[Wikipedia](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)
a.k.a.
[Bounded Quantification](https://en.wikipedia.org/wiki/Bounded_quantification#F-bounded_quantification).
<!-- TODO(emintz): document in a linked page here and below. -->

Note that nodes cannot be copied (though they can be cloned) or assigned.
Nodes have no natural order, and can only be compared for equality. Equality
is instance-based, i.e. a node can only equal itself.

*  `Node` provides basic node functionality
  * Support for factories
  * Manipulate trees, including adding, disconnecting, and excising nodes
  * Copy a node copies
  * Access node characteristics including type name, parent, children
* `Host` extends 'Node' to support `dynamic_cast`-based
  double dispatch.
* `AttrNode` extends 'Host' to support arbitrary 
  [`std::string`](https://en.cppreference.com/w/cpp/string/basic_string.html)-valued
  attributes <!-- TODO: link -->
  
## Base Attributed Node Class

<!-- TODO(emintz): link to details -->

`BaseAttrNode`, a thin subclass of 
`AttrNode<BaseAttrNode>` is a convenience `AttrNode<BaseAttrNode>` non-templated subclass
that provides convenience methods that support generated code.
It also provides the double dispatch method of last resort that supports
"if all else fails" node type-specific procesing.
