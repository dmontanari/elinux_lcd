/*

        rpilcd - Testing librpilcd i2c

    Copyright (c) 2015-2015 Daniel Montanari ( dmontanari@gmail.com )

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


*/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <lcd_hd44780u.h>

#define I2C_ADDR 0x20

int main (void)
{

        int r   =       0;

        r = lcd_init( I2C_ADDR, 1 );
        if ( r )
        {
                fprintf( stderr, "Oops... I2C says : %s [%i]\n", strerror( r ), r );
                return 0;
        }

        unsigned char i = 0;


        fprintf( stdout, "Printing some text.........."), fflush( stdout );

///                 12345678901234567890
        lcd_print( "RPi I2C LCD LIBRARY\0", 1 );
        lcd_print( "github.com/dmontanari/librpilcd\0", 2 );

        fprintf( stdout, "DONE\n" ), fflush( stdout );
        fprintf( stdout, "Sleeping for 5 seconds......"), fflush( stdout );
        sleep ( 5 );
        fprintf( stdout, "DONE\n" ), fflush( stdout );
        fprintf( stdout, "Blinking 5 times.."), fflush( stdout );

        /// "Blink" a few times
        for ( i=0; i<5; i++ )
        {
                lcd_off( );
                usleep( 250000 );
                lcd_on( );
                usleep( 250000 );
                fprintf(stdout, "%d.", i+1 ), fflush( stdout );
        }
        fprintf( stdout, "DONE\n" ), fflush( stdout );

        lcd_clear( );
        fprintf( stdout, "Testing shift to left.......");
        lcd_print( "shift left", 1 );

        for ( i = 0; i < 10; i++ ) {

                lcd_scroll_left( );
                usleep( 250000 );
        }
        fprintf( stdout, "DONE\n" );
        fprintf( stdout, "Testing shift to right......");
        for ( i=0;i<10;i++ ) {
                lcd_scroll_right( );
                usleep( 250000 );
        }
        fprintf( stdout, "DONE\n" );
        fprintf( stdout, "Sleeping for 5 seconds......"), fflush( stdout );
        sleep( 5 );
        fprintf( stdout, "DONE\n"), fflush( stdout );
        fprintf( stdout, "Clearing...................." ), fflush( stdout );
        lcd_clear( );
        fprintf( stdout, "DONE\n"), fflush( stdout );
        fprintf( stdout, "Test finishedt\n" ), fflush( stdout );


        lcd_close();

        return 0;

}

