Software Architecture {#software_architecture}
=====================

[TOC]

Objectives of the Software Architecture
---------------------------------------

The objectives of the software architecture are:

 - Designed for testability: The software parts have interfaces which allow to test those parts individually.
 - A use case driven approach: The architecture shall be centered on use cases and not depend on implementation details.
 - Flexible structure: The software shall not be hard to adjust to changing hardware, development frameworks or external/third party libraries.

Achieving a Flexible Structure {#flexible_structure}
------------------------------

\note The description will use the term '*level*' or '*policy* level'.
      It is used according to the description given in \cite CleanArchitecture p. 184 ff..

For maintaining flexibility the software architecture shall adhere to:

- The 'Acyclic Dependencies Principle' in \cite CleanArchitecture p. 112 ff..:  
  > Allow no cycles in the component dependency graph.

- The 'Dependency Rule' in \cite CleanArchitecture p. 203 ff.:  
  > Source code dependencies must point only [...] toward higher-level policies.


For this software the implementation details (lower level) are:

 - The board (hardware) including the processor.
 - Other external/3rd party libraries.
   Exceptions defined below.

The application (higher level policies) shall not depend on those implementation details.

In contrast the software will commit to the following dependencies:

 - The code may depend on a compiler which is capable to compile C++17 as defined in ISO/IEC 14882:2017.
 - The code may depend on an implementation of the C++ Standard Library as defined in ISO/IEC 14882:2017.
 - The *hardware related* code may depend on an implementation of the [ArduinoCore-API](https://github.com/arduino/ArduinoCore-API).

Implementing a Plug-in Architecture {#plugin_architecture}
-----------------------------------

\note The description will use the term '*package*'.
      In this context a package groups related implementation of functionality of a common policy level.
      It does not refer to a specific artifact type in a programming language.

### Interpretation of the Dependency Rule {#interpretation_dependency_rule}

For this software architecture the Dependency Rule (see \cite CleanArchitecture p. 203) is applied on source code dependencies between *packages*.
That means that source code may only depend on source code within its own package or on source code in packages of higher-level policies.

### Dependency Injection {#dependency_injection}

For a higher level package to use an implementation from a lower level, interfaces shall be used.
At runtime the higher level package needs to use the actual object realizing the interface.
But this shall be done without revealing the implementation details to the higher level package.
For this, non-member "getter"- or "factory"- functions (Dependency Injector Function; DIF) shall be declared in the higher level package.
Using functions as dependency injector allows to call the constructor of the implementation with run-time arguments.
The actual object is created by the DIF which is defined in the lower layer package.
The DIF provides the object to the caller, for example by returning a reference to it.

The declaration of a DIF must reside in the (highest) level which calls it.
Its definition must reside at the level which implements the dependency or at a lower level.
But it should not reside in the same unit as the dependency.
As the definition of the DIF depends on its usage it should not pollute the implementation's unit.
This simplifies reusability and testability of the implementation.

### Interface Adapters

In case an implementation detail can not be changed:
For example because it is a stable software part, or 3rd-party or otherwise immutable.
Then interface adapters must be implemented.
Those interface adapters realize the higher layer interface while using the stable component.
The usage may for example be realized using the delegation pattern.

\dotfile dependency_injection_interface_adapter.dot "example of dependency injection and interface adapter"

### Components

Components are parts of the software which may be exchanged or changed as a whole.
For example a library (including any interface adapters) can be considered as one component.
Another example of a component is the software for adapting to the board (the device's hardware used by the software).
As a different board may be used for the same application.

Also this definition of *components* from the [C4 software architecture model](https://c4model.com/) by Simon Brown applies (\cite CleanArchitecture p. 314) applies:

> A grouping of related functionality behind a nice clean interface, which resides inside an execution environment like an application.

Component Architecture {#component_architecture}
----------------------

The following diagram sketches the plan for the component architecture.
The elements in the diagram are placeholders for components which will be defined in a generic way further below.

The component architecture shall support the objectives of the software architecture.
Mechanisms for dependency injection are omitted in this diagram for readability.

\dotfile component_architecture.dot "component architecture for this software"

The rectangles with bold border are packages.
The label at the top of each package is the name of the package.
Each package consists of one or more components.
All components in this diagram are exemplarily.

The components are described in further detail in their respective pages:

 - \subpage utilities
 - \subpage enterprise_business_rules
 - \subpage application_business_rules
 - \subpage board_adapters
 - \subpage third_party_adapters

Further References
------------------

* \cite TheCleanArchitecture
