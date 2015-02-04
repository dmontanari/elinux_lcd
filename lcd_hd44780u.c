/*
    This file is part of librpilcd.

    Copyright (c) 2015-2015 Daniel Montanari ( dmontanari@gmail.com )

    Foobar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with librpilcd.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/ioctl.h>


/// Commands
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

// flags for function set
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00

// flags for backlight control
#define LCD_BACKLIGHT           0x08
#define LCD_NOBACKLIGHT         0x00
//////////////////////////////////


#define         _RS             ( 1 << 6 )                      // P6 of PCF8574
#define         _RW            ( 1 << 5 )                      // P5 of PCF8574
#define         _E               ( 1 << 4 )                      // P4 of PCF8574

/// Data bits ( not ports ! )
#define         _DB7            ( 1 << 7 )                      // P3
#define         _DB6            ( 1 << 6 )                      // P2
#define         _DB5            ( 1 << 5 )                      // P1
#define         _DB4            ( 1 << 4 )                      // P0
#define         _DB3            ( 1 << 3 )
#define         _DB2            ( 1 << 2 )
#define         _DB1             ( 1 << 1 )
#define         _DB0             (1 << 0 )


#define MAXDEV_NAME     0xff

struct lcd_driver
{

    int           fd;
    char        device_address;
    char        device_name[MAXDEV_NAME];

};

struct lcd_driver lcd = {

                                .fd = -1,
                                .device_address = 0,

                        };

static int write_to_display( char data ) {

        unsigned char retry = 0;

        /// Do 10 retries because of the
        /// I2C clock stretching problem
        /// Check https://github.com/raspberrypi/linux/issues/254
        while ( write( lcd.fd, &data, 1 ) != 1 )  {

                usleep( 1000 );
                if ( ++retry == 10 ) {

                        fprintf( stderr, "Retry Overflow : Error sending data %s\n", strerror( errno ) );
                        exit( EXIT_FAILURE );
                }

        }

        return 0;

}

static void strobe( const char data ) {

        /// Strobe bit _E
        write_to_display( data | _E );
        usleep( 1500 );
        write_to_display( data );
        usleep( 1500 );

}

static void write_4bits( char data ) {

        /// Read Busy Flag :)

        strobe( data >> 4 );
        strobe( data & 0x0F );
        write_to_display( 0x00 );

}

static void lcd_putchar( char data ) {

        strobe( LCD_SETCGRAMADDR | (data >> 4 ) );
        strobe( LCD_SETCGRAMADDR | (data & 0x0F ) );

}

//////////////////////////////////////////////////////////////////////////
///
///                     Library functions
///

int lcd_init( char device_address, char channel ) {

        /// Open the device
        lcd.fd = -1;
        lcd.fd = open("/dev/i2c-0", O_RDWR);
        if (lcd.fd  < 0)
        {
                // Permission denied - don't need to try i2c-1
                if ( errno == EPERM )
                        return errno;

                if ( channel == 0 )
                        lcd.fd = open("/dev/i2c-0", O_RDWR);
                else
                        lcd.fd = open("/dev/i2c-1", O_RDWR);

                if ( lcd.fd < 0 )
                        return errno;

        }

        sprintf( lcd.device_name, "HD44780U" );
        lcd.device_address = device_address;

        /// Set slave address
        if (ioctl( lcd.fd, I2C_SLAVE, lcd.device_address ) < 0) {

                printf("ioctl error: %d - %s\n", errno, strerror(errno));
                close(lcd.fd);
                return errno;

        }

        /// Page 42 of HD44780 datasheet
        /// Initialize for 4 bits operations
        /// https://www.sparkfun.com/datasheets/LCD/HD44780.pdf

        /// Wait 15 ms
        /// Send B5 | DB4
        /// Wait min. 4100 uS
        /// Send B5 | B4
        /// Wait min. 4100 uS
        /// Send B5 | B4
        /// Wait min. 100 uS
        /// Send B5 | B4
        write_to_display( 0x00 );
        usleep( 15000 );

        strobe( 0x03 );
        usleep( 5000 );

        strobe( 0x03 >> 4 );
        usleep( 5000 );

        strobe( 0x03 );
        usleep( 5000 );

        strobe( 0x02 );
        usleep( 5 );

        /////////////////////////////////
        /// We are in 4-bits now
        write_4bits( LCD_FUNCTIONSET | LCD_2LINE | LCD_5x8DOTS );
        usleep( 5000 );

        /// Display on, cursor off
        write_4bits( LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF );
        usleep( 5000 );

        /// Clear lcd
        write_4bits( LCD_CLEARDISPLAY );
        usleep( 5 );

        /// entry mode
        write_4bits( LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT );

        return 0;

}

/// Close the file descriptor
void lcd_close()
{

        if ( lcd.fd != -1 )
                close( lcd.fd );

}



void lcd_print_position( const char *data, unsigned char line, unsigned char col )
{

        const char *d = data;
        unsigned char cnt = 0;

        switch( line ) {

                case 1:
                        write_4bits( 0x80 + col );
                        break;
                case 2:
                        write_4bits( 0xC0 + col );
                        break;
                default:
                        /// \TODO: error handler
                        fprintf( stdout, "Invalid line number [%d]\n", line );
                        exit( EXIT_FAILURE );
                        break;
        }

        while ( *d ) {

                lcd_putchar( *d );
                d++;
                /// 16 x 2
                if ( ++cnt == 20 )
                        break;
        }

        /// Set _RS down
        write_to_display( 0x00 );

}


void lcd_print( const char *data, unsigned char line ) {

        lcd_print_position( data, line, 0 );

}


void lcd_off( ) {

        write_4bits( LCD_DISPLAYCONTROL | LCD_DISPLAYOFF );

}

void lcd_on( ) {

        write_4bits( LCD_DISPLAYCONTROL | LCD_DISPLAYON );

}

void lcd_clear( ) {

        write_4bits( LCD_CLEARDISPLAY );
        lcd_off( );
        lcd_on( );

}

void lcd_scroll_left( )
{

        write_4bits( LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT );

}

void lcd_scroll_right( )
{

        write_4bits( LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT );

}
