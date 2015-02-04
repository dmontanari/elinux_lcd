
                        . : elinux_lcd : .

Library for handle LCD devices using i2c interface (PCF8574)

Tested with Raspberry PI model B and HD4478U 20x2 LCD display

       . : Connections : .

        LCD     PCF8574
        ---------------
        RS      P6
        RW      P5
        E       P4
        DB7     P3
        DB6     P2
        DB5     P1
        DB4     P0
        DB3     Don't connect
        DB2     Don't connect
        DB1     Don't connect
        DB0     Don't connect

------ Instructions ------

1 - Clone the repository

        $ git clone https://github.com/dmontanari/elinux_lcd

2 - Change dir

        $ cd elinux_lcd

3 - Build and install the lib

        $ make lib

        $ sudo make install_lib

4 - Build the demo

        $ make demo


5 - Load some modules

        $ sudo modprobe i2c-bcm2708

        $ sudo modprobe i2c-dev


6 - Execute the demo

        $ sudo ./lcd_demo

7 - To remove the library, enter the lib directory and do a sudo make uninstall_lib

Files installed:

/usr/local/include/lcd_hd44870u.h
/usr/local/lib/libelinux_lcd.so

                . : Usage : .

Compile your program with -lelinux_lcd



Happy Hack :)


        -------------------------------------------------------
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
        -------------------------------------------------------


