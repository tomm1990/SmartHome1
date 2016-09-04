the run the program, unzip the files and open the terminal in the folder, then run:

1) sudo mknod /dev/cdev_example c 245 0	
   change the file permmisions of step 1 to 7777
2) make
3) sudo insmod CharDev.ko
4) gcc ioctl_user.c -o a.out

to run the program :
./a.out arg1 arg2

arg1 is the exsiting file you want to backup and restore.
arg2 is the restored file.

the program erase arg1.

i tried to write a makefile that do all of this , but i didn't have more time to write a better makefile.

if i had more time to make it better i would use modulus operator to calculate exactly the tapes needed:

tapes=fileSize/10
offset=fileSize%10

and then i could split any file less then the total space of the tape

the program works fine as long as you tried to backup and restore file less then 2000 bytes.
you can try to run it on the hello.txt file


