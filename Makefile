CC = gcc
TARGET_LIB = libelinux_lcd.so
TARGET_HEADERS = lcd_hd44780u.h

lib:
	$(CC) -c -Wall -Werror -fPIC lcd_hd44780u.c
	$(CC) -shared -o $(TARGET_LIB) lcd_hd44780u.o

clean_lib:
	rm -rfv lcd_hd44780u.o *.so


demo:
	$(CC) -o lcd_demo main.c -lelinux_lcd

install_lib:
	cp $(TARGET_LIB) /usr/local/lib/
	cp $(TARGET_HEADERS) /usr/local/include


uninstall_lib:
	rm -fv /usr/local/lib/$(TARGET_LIB)
	rm -fv /usr/local/include/$(TARGET_HEADERS)

