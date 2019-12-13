<h3 align="center">Heterogeneous factory</h3>

<div align="center">

  [![Status](https://img.shields.io/badge/status-active-success.svg)]()
  [![License](https://img.shields.io/badge/license-MIT-blue.svg)](/LICENSE)

</div>

---

<p align="left"> Header only heterogeneous factory pattern with automated static registration
    <br>
</p>

## About <a name = "about"></a>
This library provide you a heterogeneous factory with a very convinient automated static registraion.
```c++
// Interface.h
struct Interface { ... };

// InterfaceFactory.h
using InterfaceFactory = HGSFactory<Interface>;

// Concrete.h
struct Concrete : public Interface {
    Concrete(int, int) {}
    static std::string factoryRegistrationName() { return "concrete"; }
};

// Concrete.cpp
REGISTER_IN_FACTORY_STATIC(InterfaceFactory, Concrete, int, int)

// ConcreteSecond.h
struct ConcreteSecond : public Interface {
    Concrete(int) {}
    static std::string factoryRegistrationName() { return "concrete_second"; }
};

// ConcreteSecond.cpp
REGISTER_IN_FACTORY_STATIC(InterfaceFactory, ConcreteSecond, int)

// logic.cpp
auto concrete = InterfaceFactory::create("concrete", 2, 3);
auto concreteSecond = InterfaceFactory::create("concrete_second", 10);
```

Please see the [test](https://github.com/farmovit/heterogeneous_factory/blob/master/test/factory_test.cpp) to get more information.
Also documentations are available in source files.

### Requirements
```
CMake 3.6
C++17
```

