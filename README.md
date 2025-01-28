# SAT (SWISS ARMY TOOL)

This library has the purpose of making it easier to use some resources frequently used, such as databases, utilities, and communication. The concept here applied is evolutionary computation, where it is very important to always keep in mind that all implementations must be thought of as being used wherever and whenever we want. This means each implementation must be analyzed to become a library. The language used to create it is C. Why the C language? The C language is a powerful tool where it is possible to create anything with no restrictions; it is easy to learn but hard to master.


## Resources

## Compiling

### Enabling a specific module
```bash
mkdir build && cd build
cmake -D<module> ..
```

### Enabling all

```bash
mkdir build && cd build
cmake -DSAT_MYSQL=ON -DSAT_POSTGRES=ON -DSAT_SQLITE=ON -DSAT_KAFKA=ON -DSAT_CURL=ON -DSAT_REDIS=ON -DSAT_WEBSERVER=ON -DSAT_JSON=ON -DSAT_MQTT=ON -DSAT_PROPERTIES=ON -DSAT_SDL=ON -DSAT_GTK3=ON -DSAT_OPENGL=ON ..
```

### Utilities
* UUID
* Log
* Event
* File
* Status
* Time
* Validation

### Databases
* SQLite
* MySQL
* Postgres
* redis
### Communications
* UDP
* Kafka
### Data structures
* Array
* Stack
* Linked List
* Queue
* Map

### Next 
* Tree
* TCP
* MQTT
* SDL
* GTK
* Ini
* curl
* IPC Shared Memory System V
* IPC Queue System V
* IPC Semaphore System V

