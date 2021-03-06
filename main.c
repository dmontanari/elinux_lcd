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
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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



        printf( "ELINUX RPi I2C LIB\n" );
        printf( "I2C with PCF8574\n");
///                 12345678901234567890
        lcd_print( " ELINUX RPi I2C LIB", 1 );
        lcd_print( "  I2C with PCF8574", 2 );
        printf( "Sleeping for 5 seconds......");
        sleep ( 5 );
        printf( "DONE\n" );

        printf( "It may blink\n");
        printf( "Turning off and on\n" );

        lcd_clear();
///                 12345678901234567890
        lcd_print( "    It may blink", 1 );
        lcd_print( " Turning off and on", 2 );
        /// "Blink" a few times
        for ( i=0; i<5; i++ )
        {
                lcd_off( );
                usleep( 350000 );
                lcd_on( );
                usleep( 350000 );
                fprintf(stdout, "%d.", i+1 ), fflush( stdout );
        }

        lcd_clear( );
        printf( "Scroll\n");
///                 12345678901234567890
        lcd_print( "Scroll", 1 );

        for ( i=0;i<14;i++ ) {
                lcd_scroll_right( );
                usleep( 250000 );
        }

        for ( i = 0; i < 14; i++ ) {
                lcd_scroll_left( );
                usleep( 250000 );
        }

        lcd_clear();
        printf( "Show system info\n" );
        lcd_print( "Show system info", 1 );
        sleep( 5 );

        /// Show core temperature
        /// $ cat /sys/class/thermal/thermal_zone0/temp
        /// 47078
        ///
        char temperature[6];
        int fd;

        fd = open( "/sys/class/thermal/thermal_zone0/temp", O_RDONLY );
        if ( fd == -1 ) {

                fprintf(stderr, "Open temperature sensor fail %s\n", strerror( errno ) );
                lcd_close();
                exit( EXIT_FAILURE );

        }
        temperature[5] = 0;     /// set NTZ

        if ( read( fd, temperature, 5 ) == -1 ) {

                fprintf(stderr, "Read temperature sensor fail %s\n", strerror( errno ) );
                lcd_close();
                close(fd);
                exit( EXIT_FAILURE );

        }

        lcd_clear( );
        lcd_print( "Temperature", 1 );  /// 11 chars
        lcd_print_position( temperature, 1, 12 );

        sleep( 5 );
        lcd_clear();
        lcd_print( "Whatever you want", 1 );

        sleep(5);
        lcd_clear();
        lcd_print( " clone me at github", 1 );
        lcd_print( "http://goo.gl/fGLxqw", 2);

        lcd_close();

        return 0;

}

