8bithouse documentation
=======================

### Commands (Encoded with 3 bits)

* add 		-> 000 		Add device
* rm 		-> 001 		Remove device
* enable 	-> 010		Enable device
* disable 	-> 011		Disable device
* reset 	-> 100 		Reset all devices
* list		-> 101		Print list of all devices added
* dim 		-> 110		Reglate brightness of enabled device

Acknowledge (Message returned by arduino board): is sent as a normal message passed and quickly printed by client on the shell


### Server

While (1):	

* Check for interrupt from serial
    * Waiting for message from client
    * Receiving message from client -> Decoding
    * Message control
    * Managing command -> Editing of data structures
    * Managing devices
    * Sending acknowledge message -> Display message on display and/or sound from speaker
* Output updated values

	
Data structures:

1. 8 bit bitmap for initialized devices (1: initialized / 0: not intialized)
2. 8 bit bitmap for active devices (1: active / 0: not active)
3. Array of devices names ("name": initialized / "": not initialized)
4. Array of output values for dimmer (from 0 to 255)
5. Variable for buzzer (1: enabled / 0: disabled)
6. Array of comands already initialized (<3 bits code>: function pointer to proper function)


### Client

Message structure: 

* Command code
* Device name (No name for reset and list)
	 
Sends message and waits for a reply from server
	

### Functions

* ADD: Search for a hole in the bitmap of initialized devices. If server finds a free space, it fill the hole with a 1 and the corresponding cell of the array of names with the string received from client. At first the device is disabled.
* RM: Search for the name received and if server finds it, the bitmap is freed with a 0 and the name erased from array
* ENABLE: If found, device enabled in the bitmap
* DISABLE: If found, device disabled in bitmap
* RESET: Frees all data structures
* DIM: Search for the name received and if server finds it, waits for + or - values as command for increasing and decresing brightness of the device


### How to compile and run

* Arduino -> Enter "arduino" folder > type "make" > type "make 8bh_avr.hex"
* Client -> Enter "client" folder > type "make" > type "./8bh" to run
