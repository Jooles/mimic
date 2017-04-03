# MIMIC 
[![Build Status](https://travis-ci.org/Jooles/mimic.svg?branch=master)](https://travis-ci.org/Jooles/mimic)
[![Documentation](https://codedocs.xyz/Jooles/mimic.svg)](https://codedocs.xyz/Jooles/mimic/)

## A clean-room JVM implementation in C++14

Why, you ask? Well C++98 is great and all but there comes a time in every developer's life when they need to knuckle down and catch up to current standards. I needed a project to work on and this is it!

I have no expectations that this will work or that it'll be good, but it's an educational tool so let's see what happens. As some people I know have expressed an interest, I'll be blogging about the process of writing this over at https://jooles.github.io/mimic

### Prerequisites
* C++ compiler that supports -std=c++14 and the filesystem TS (CI builds performed with GCC 5 and 6)
* libpthread (for tests)

C++ standard support will change to C++ 17 as support becomes more readily available

### Credit
Oracle and Java are registered trademarks of Oracle and/or its affiliates. Other names may be trademarks of their respective owners.
