#!/bin/sh
cd "/mnt/4/Projects/Programming/C++/Apps/MercurySteam/app/build/linux/x86_64/debug"
export LD_LIBRARY_PATH="/mnt/4/Projects/Programming/C++/Apps/MercurySteam/app/build/linux/x86_64/debug"
while(true)
do
  ./MercurySteam --docroot docroot --approot approot --http-address 0.0.0.0 --http-port 8080;
done
