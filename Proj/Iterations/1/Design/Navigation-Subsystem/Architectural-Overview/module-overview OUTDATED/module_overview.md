# **Index**
- [**Index**](#index)
- [**CFL: *Core Functionality Layer***](#cfl-core-functionality-layer)
	- [**HMI: *Human-Machine Interface***](#hmi-human-machine-interface)
		- [**Enumerations**](#enumerations)
		- [**Structures**](#structures)
		- [**Attributes:** Static](#attributes-static)
		- [**Attributes:** Instance](#attributes-instance)
		- [**Methods**](#methods)
		- [**Events**](#events)
		- [**Typical Use Case**](#typical-use-case)
	- [**DC: *Digital Control***](#dc-digital-control)
		- [**Enumerations**](#enumerations-1)
		- [**Structures**](#structures-1)
		- [**Attributes:** Static](#attributes-static-1)
		- [**Attributes:** Instance](#attributes-instance-1)
		- [**Methods**](#methods-1)
		- [**Events**](#events-1)
		- [**Typical Use Case**](#typical-use-case-1)
	- [**COM: *Communications***](#com-communications)
		- [**Enumerations**](#enumerations-2)
		- [**Structures**](#structures-2)
		- [**Attributes:** Static](#attributes-static-2)
		- [**Attributes:** Instance](#attributes-instance-2)
		- [**Methods**](#methods-2)
		- [**Events**](#events-2)
		- [**Typical Use Case**](#typical-use-case-2)
- [**HLHAL: *High-Level Hardware Abstraction Layer***](#hlhal-high-level-hardware-abstraction-layer)
	- [**DIO: *Digital Input/Output***](#dio-digital-inputoutput)
		- [**Enumerations**](#enumerations-3)
		- [**Structures**](#structures-3)
		- [**Attributes:** Static](#attributes-static-3)
		- [**Attributes:** Instance](#attributes-instance-3)
		- [**Methods**](#methods-3)
		- [**Events**](#events-3)
		- [**Typical Use Case**](#typical-use-case-3)
	- [**US: *Ultrasonic Sensor***](#us-ultrasonic-sensor)
		- [**Enumerations**](#enumerations-4)
		- [**Structures**](#structures-4)
		- [**Attributes:** Static](#attributes-static-4)
		- [**Attributes:** Instance](#attributes-instance-4)
		- [**Methods**](#methods-4)
		- [**Events**](#events-4)
		- [**Typical Use Case**](#typical-use-case-4)
	- [**MD: *Motor Driver***](#md-motor-driver)
		- [**Enumerations**](#enumerations-5)
		- [**Structures**](#structures-5)
		- [**Attributes:** Static](#attributes-static-5)
		- [**Attributes:** Instance](#attributes-instance-5)
		- [**Methods**](#methods-5)
		- [**Events**](#events-5)
		- [**Typical Use Case**](#typical-use-case-5)
	- [**CE: *Communications Engine***](#ce-communications-engine)
		- [**Enumerations**](#enumerations-6)
		- [**Structures**](#structures-6)
		- [**Attributes:** Static](#attributes-static-6)
		- [**Attributes:** Instance](#attributes-instance-6)
		- [**Methods**](#methods-6)
		- [**Events**](#events-6)
		- [**Typical Use Case**](#typical-use-case-6)
- [**LLHAL: *Low-Level Hardware Abstraction Layer***](#llhal-low-level-hardware-abstraction-layer)

# **CFL: *Core Functionality Layer***

## **HMI: *Human-Machine Interface***


### **Enumerations**

### **Structures**

### **Attributes:** Static

### **Attributes:** Instance

### **Methods**

### **Events**

### **Typical Use Case**




## **DC: *Digital Control***


### **Enumerations**

### **Structures**

### **Attributes:** Static

### **Attributes:** Instance

### **Methods**

### **Events**

### **Typical Use Case**




## **COM: *Communications***


### **Enumerations**

### **Structures**

### **Attributes:** Static

### **Attributes:** Instance

### **Methods**

### **Events**

### **Typical Use Case**




# **HLHAL: *High-Level Hardware Abstraction Layer***

## **DIO: *Digital Input/Output***


### **Enumerations**

### **Structures**

### **Attributes:** Static

### **Attributes:** Instance

### **Methods**

### **Events**

### **Typical Use Case**




## **US: *Ultrasonic Sensor***


### **Enumerations**

### **Structures**

### **Attributes:** Static

### **Attributes:** Instance

### **Methods**

### **Events**

### **Typical Use Case**




## **MD: *Motor Driver***


### **Enumerations**

### **Structures**

### **Attributes:** Static

### **Attributes:** Instance

### **Methods**

### **Events**

### **Typical Use Case**




## **CE: *Communications Engine***


### **Enumerations**

### **Structures**

### **Attributes:** Static

### **Attributes:** Instance

### **Methods**

### **Events**

### **Typical Use Case**
# **LLHAL: *Low-Level Hardware Abstraction Layer***

This layer of software is the only direct interface that the program has with the **BSP (*Board Support Package*)** and so it is meant to facilitate development in the layers that go above it and **make it easier to port** the code into a physical platform. It should thus be **different from platform to platform** as much as is needed to satisfy the requirements of the [*HLHAL*](#hlhal-high-level-hardware-abstraction-layer).
