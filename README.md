<h3 align="center">Heterogeneous factory</h3>

<div align="center">

  [![Status](https://img.shields.io/badge/status-active-success.svg)]()
  [![License](https://img.shields.io/badge/license-MIT-blue.svg)](/LICENSE)

</div>

---

<p align="center"> Header only heterogeneous factory pattern with automated static registration
    <br>
</p>

## About <a name = "about"></a>
This library provide you a heterogeneous factory with a very convinient automated static registraion.
```c++
// Interface.h
struct Interface { ... };

// InterfaceFactory.h
using InterfaceFactory = HGSFactory<Interface>;

// ConcreteInterface.h
struct ConcreteInterface : public Interface {
    ConcreteInterface(int, int) {}
    static std::string factoryRegistrationName() { return "concrete"; }
};

// ConcreteInterface.cpp
REGISTER_IN_FACTORY_STATIC(ConcreteInterface, InterfaceFactory, int, int)

// logic.cpp
auto concrete = InterfaceFactory::create("concrete", 2, 3);
```

### Requirements
```
CMake 3.6
C++17
```

## 🎈 Usage <a name="usage"></a>
Please see the [test](https://github.com/farmovit/QPointerGrabber/blob/master/test/GrabberTest.cpp) to get how it can be used.
