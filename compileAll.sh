cd gpio_driver
make clean
make
sudo insmod gpio_driver.ko
sudo mknod /dev/gpio_driver c 243 0
sudo chmod 666 /dev/gpio_driver

cd ..
gcc -pthread -o test getch.c main.c
