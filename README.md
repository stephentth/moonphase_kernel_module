# moonphase linux kernel module

A loadable kernel module for Linux that provides the functionality to display the current moon phase through a character device (`/dev/moonphase`).

Disclaimer: Moon phase calculation function may be inaccurate. Not intended for astrogation, or for treatment or management of lycanthropy.


# Build

```
make
```


# Usage

```
# install module
$ sudo insmod moonphase.ko

$ cat /dev/moonphase          
Waning Crescent Moon 🌘
            .----------.            
        .--'   o    .   `--.        
      .'@  @@@@@@ O   .   . `.      
    .'@@  @@@@@@@@   @@@@   . `.    
  .'    . @@@@@@@@  @@@@@@    . `.  
 / @@ o    @@@@@@.   @@@@    O   @\ 
 |@@@@               @@@@@@     @@| 
/ @@@@@   `.-.    . @@@@@@@@  .  @@\
| @@@@   --`-'  .  o  @@@@@@@      |
|@ @@                 @@@@@@ @@@   |
\      @@    @   . ()  @@   @@@@@  /
 |   @      @@@         @@@  @@@  | 
 \  .   @@  @\  .      .  @@    o / 
  `.   @@@@  _\ /     .      o  .'  
    `.  @@    ()---           .'    
      `.     / |  .    o    .'      
        `--./   .       .--'        
            `----------'            


# unload module
$ sudo rmmod moonphase
```
