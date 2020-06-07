# **Index**
- [**Index**](#index)
- [**Observations**](#observations)
- [**MAL: *Main Application Layer***](#mal-main-application-layer)
	- [**General**](#general)
		- [**HMI**](#hmi)
		- [**Control**](#control)
		- [**COM**](#com)
		- [**OS**](#os)
	- [**IO** Sublayer](#io-sublayer)
		- [**IO**](#io)
		- [**IO -> IRSensor**](#io---irsensor)
		- [**IO -> Motor**](#io---motor)
		- [**IO -> DigitalIO**](#io---digitalio)
	- [**COM** Sublayer](#com-sublayer)
		- [**COMRedundancyEngine**](#comredundancyengine)
		- [**COMStream**](#comstream)
- [**AHAL: *Application Hardware Abstraction Layer***](#ahal-application-hardware-abstraction-layer)
	- [**General**](#general-1)
		- [**GPIO**](#gpio)
		- [**Timer**](#timer)
	- [**OS** Sublayer](#os-sublayer)
		- [**SharedMemory**](#sharedmemory)
		- [**Thread**](#thread)
		- [**ThreadScheduler**](#threadscheduler)
		- [**Mutex**](#mutex)
	- [**LLCOM** Sublayer](#llcom-sublayer)
		- [**LLCOM**](#llcom)
		- [**LLCOM -> Bluetooth**](#llcom---bluetooth)
		- [**LLCOM -> Serial**](#llcom---serial)

# **Observations**

- Inheritance is to be used inside certain sublayers and related classes and structures with the obvious benefits but there should be as little nesting as possible to avoid runtime performance issues.
- Inheritance is represented in the respective class headers by 'Parent   -> Child'.
- Setter and getter methods for in-class members that have obvious or no restrictions are replaced by making said  members public to reduce workload in development and improve runtime performance. 

# **MAL: *Main Application Layer***

## **General**

### **HMI**


### **Control**


### **COM**


### **OS**


## **IO** Sublayer

### **IO**

### **IO -> IRSensor**

### **IO -> Motor**

### **IO -> DigitalIO**


## **COM** Sublayer

### **COMRedundancyEngine**

### **COMStream**



# **AHAL: *Application Hardware Abstraction Layer***

## **General**

### **GPIO**


### **Timer**


## **OS** Sublayer

### **SharedMemory**

### **Thread**

### **ThreadScheduler**

### **Mutex**


## **LLCOM** Sublayer

### **LLCOM**

### **LLCOM -> Bluetooth**

### **LLCOM -> Serial**

