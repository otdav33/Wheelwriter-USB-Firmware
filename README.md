# Wheelwriter-USB-Firmware
Keyboard firmware for Arduino (Teensy LC) to have a wheelwriter keyboard connect to a computer via USB

This uses the colemak layout. It should be a small modification to make it QWERT.

Edit these lines to use the pins you have attached to the ribbons:

`const int rows[8] = {13, 14, 15, 16, 17, 18, 19, 20};

const int cols[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}`

The rows use the bigger tab, even though it has less active pins. The rest just don't lead to anything.
