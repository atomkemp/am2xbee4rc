# am2xbee4rc
Convert your old AM RC equipment using XBee and Arduino

Using Arduino and XBee technology as the replacement guts for old RC equipment is a quick and easy way to breathe new life into your old toys.

Code has been thrown together rather quickly and most certianly can be improved upon. The transmitter waits for an "OK" response from the receiver and binds itself. Each packet is preceded with three 0x01's indicating the start of the packet. Flow is dictated by the speed of the Fio's ability to interpret and respond to packets. Feel free to modify code as you see fit. Enjoy!
