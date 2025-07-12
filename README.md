# SAT (SWISS ARMY TOOL)

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/solidcris/sat)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Language](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform](https://img.shields.io/badge/platform-Linux-lightgrey.svg)](https://www.linux.org/)

**SAT (Swiss Army Tool)** is a comprehensive C library designed to simplify the use of commonly required resources in system programming, including databases, utilities, network communication, GUI frameworks, and data structures. The library follows evolutionary computation principles, ensuring that all implementations are designed to be reusable, portable, and maintainable.

## 🎯 Purpose

The SAT library addresses the need for a unified, well-tested, and efficient collection of system programming tools. Rather than reinventing the wheel for each project, SAT provides battle-tested implementations that can be used across different applications and platforms.

**Why C?** The C language is chosen for its power, portability, and universal compatibility. It allows creating anything with no restrictions while maintaining high performance and broad compatibility across systems.

## 📚 Table of Contents

- [Features](#-features)
- [Installation](#-installation)
- [Core Modules](#-core-modules)
- [Optional Modules](#-optional-modules)
- [API Documentation](#-api-documentation)
- [Contributing](#-contributing)
- [License](#-license)

## 🚀 Features

### Core Library Features
- **Memory Safe**: Comprehensive memory management with leak detection
- **Thread Safe**: Multi-threading support with proper synchronization
- **Event Driven**: Asynchronous programming support with callbacks
- **Modular Design**: Use only what you need through optional modules
- **Cross Platform**: Linux-focused with portable design principles
- **Well Documented**: Comprehensive API documentation and examples

### Performance Features
- **Efficient Memory Management**: Direct buffer access and shared memory for minimal copying
- **Minimal Dependencies**: Lean core with optional external dependencies
- **Optimized Data Structures**: High-performance implementations
- **Resource Pooling**: Efficient resource management and reuse

## 🛠 Installation

### Prerequisites
- GCC or Clang compiler
- CMake 3.16 or higher
- Make build system
- Linux operating system (tested on Ubuntu, CentOS, Arch)

### Basic Installation

```bash
git clone https://github.com/solidcris/sat.git
cd sat
mkdir build && cd build
cmake ..
make
sudo make install
```

### Installing with All Modules

```bash
mkdir build && cd build
cmake -DSAT_MYSQL=ON \
      -DSAT_POSTGRES=ON \
      -DSAT_SQLITE=ON \
      -DSAT_KAFKA=ON \
      -DSAT_CURL=ON \
      -DSAT_REDIS=ON \
      -DSAT_WEBSERVER=ON \
      -DSAT_JSON=ON \
      -DSAT_MQTT=ON \
      -DSAT_PROPERTIES=ON \
      -DSAT_SDL=ON \
      -DSAT_GTK3=ON \
      -DSAT_OPENGL=ON ..
make
sudo make install
```

### Installing Specific Modules

```bash
mkdir build && cd build
cmake -DSAT_MYSQL=ON -DSAT_JSON=ON ..
make
sudo make install
```



## 🧩 Core Modules

### System Utilities
| Module | Description | Status |
|--------|-------------|--------|
| **sat_uuid** | UUID generation and manipulation | ✅ Stable |
| **sat_log** | Flexible logging system with levels | ✅ Stable |
| **sat_file** | File operations and utilities | ✅ Stable |
| **sat_time** | Time handling and formatting | ✅ Stable |
| **sat_status** | Status and error handling | ✅ Stable |
| **sat_validation** | Input validation utilities | ✅ Stable |
| **sat_event** | Event-driven programming support | ✅ Stable |

### Data Structures
| Module | Description | Features |
|--------|-------------|----------|
| **sat_array** | Dynamic arrays with automatic resizing | Type-safe, Memory-efficient |
| **sat_stack** | LIFO stack implementation | Thread-safe operations |
| **sat_queue** | FIFO queue implementation | Multiple queue types |
| **sat_linked_list** | Doubly-linked list | Iterator support |
| **sat_map** | Hash map implementation | Key-value storage |
| **sat_set** | Mathematical set operations | Unique elements |
| **sat_cache** | LRU cache implementation | Configurable size |
| **sat_iterator** | Generic iterator interface | Uniform iteration |

### Network Communication
| Module | Description | Features |
|--------|-------------|----------|
| **sat_udp** | UDP client/server | Multicast, Event-driven |
| **sat_tcp** | TCP client/server | Async/Interactive modes |

### Process Management
| Module | Description | Features |
|--------|-------------|----------|
| **sat_worker** | Worker thread management | Thread pools, Task queues |
| **sat_scheduler** | Task scheduling system | Cron-like scheduling |
| **sat_process** | Process management | Spawn, monitor, control |
| **sat_channel** | Inter-process communication | Message passing |

### IPC (Inter-Process Communication)
| Module | Description | Features |
|--------|-------------|----------|
| **sat_shared_memory** | System V shared memory | Safe memory sharing |
| **sat_queue_ipc** | System V message queues | Message passing |
| **sat_plugin** | Dynamic plugin system | Runtime loading |

## 🔧 Optional Modules

### Database Connectivity
| Module | Description | Dependencies |
|--------|-------------|--------------|
| **sat_mysql** | MySQL database connectivity | libmysqlclient |
| **sat_postgres** | PostgreSQL database connectivity | libpq |
| **sat_sqlite** | SQLite database connectivity | libsqlite3 |
| **sat_redis** | Redis database connectivity | hiredis |

### Web and API
| Module | Description | Dependencies |
|--------|-------------|--------------|
| **sat_webserver** | HTTP server implementation | civetweb |
| **sat_curl** | HTTP client functionality | libcurl |
| **sat_json** | JSON parsing and generation | json-c |
| **sat_properties** | Configuration file handling | iniparser |

### Messaging and Communication
| Module | Description | Dependencies |
|--------|-------------|--------------|
| **sat_kafka** | Apache Kafka integration | librdkafka |
| **sat_mqtt** | MQTT messaging protocol | libmosquitto |

### Graphics and GUI
| Module | Description | Dependencies |
|--------|-------------|--------------|
| **sat_gtk3** | GTK3 GUI framework | libgtk-3 |
| **sat_opengl** | OpenGL graphics programming | OpenGL, GLFW, GLEW |
| **sat_sdl** | SDL multimedia framework | libsdl2 |



## 📖 API Documentation

### Function Naming Convention
- `sat_<module>_<action>()` - Standard function format
- `sat_<module>_create()` - Create and initialize objects
- `sat_<module>_open()` - Open connections/resources
- `sat_<module>_close()` - Close connections/resources
- `sat_<module>_destroy()` - Cleanup and free memory

### Error Handling
All functions return `sat_status_t` for consistent error handling:

```c
sat_status_t status = sat_some_function ();
if (!sat_status_get_result (&status))
{
    sat_log_error ("Operation failed: %s", sat_status_get_message (&status));
    return -1;
}
```

### Memory Management
- **RAII Pattern**: Resources are automatically managed
- **Explicit Cleanup**: Always call corresponding cleanup functions
- **Memory Safety**: Built-in leak detection and prevention
- **User Responsibility**: Some functions require manual buffer management (documented)

## 🧪 Testing

Run the test suite:

```bash
cd build
make test
```

Individual module tests:
```bash
cd build
./tests/test_sat_array
./tests/test_sat_udp
./tests/test_sat_worker
```

## 🤝 Contributing

We welcome contributions! Please see our [Contributing Guide](CONTRIBUTING.md) for details.

### Code Style
- Use meaningful variable names
- Comment complex algorithms
- Include function documentation
- Maintain consistent error handling

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🔗 Links

- **Documentation**: [API Reference](docs/api.md)
- **Examples**: [Sample Code](samples/)
- **Issues**: [Bug Reports](https://github.com/solidcris/sat/issues)
- **Discussions**: [Community Forum](https://github.com/solidcris/sat/discussions)

## 🙏 Acknowledgments

- Thanks to all contributors who have helped improve SAT
- Inspired by the Unix philosophy of small, composable tools
- Built with modern C best practices and design patterns

---

**SAT** - Making system programming simpler, one module at a time. 🛠️

