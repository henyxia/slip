# The SLIP Project

### What is the SLIP Project ?

The SLIP Project, or Serial Line Internet Protocol, is a project about a connected T shirt communicating over a network using the SLIP protocol and some XBees

### Awesome !

Yes, we know !

### But, what's the point ?

This project's aim is to allow someone falling to get help quickly. By using our connected T shirt, bunch of informations will be send to a remote software that will monitor any unexpected movements (like falling).

### WoW ! That's could be useful !

Well, thank you !

### And, how does this all thing communicate ?

That's the fun part ! We get all the data with some accelerators and a temperature sensor and we send them to a LillyPad. This LillyPad send the data to an XBee. Another XBee recieve the data and transmit it to the broadcast using our script. Once the data is in the broadcast, the main software read this data and archive it. So basically, the transmission chain is the following.

Sensors <-> LillyPad <-> XBee 1 <-> XBee 2 <-> Script <-> Broadcast <-> Software
