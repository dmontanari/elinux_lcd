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
#ifndef __HD47780U_H
#define __HD4778U_H

int lcd_init( char device_address, char channel );

void lcd_close();

void lcd_print( const char *data, unsigned char line );

void lcd_clear( );

void lcd_off( );

void lcd_on( );

void lcd_scroll_left( );

void lcd_scroll_right( );


#endif
