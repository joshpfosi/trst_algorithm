By: Joshua Pfosi Alex Tong, Denali Rao

Main
====

Parses ./trst arguments to determine from where to take input (stdin or file)

Input
=====

This module is responsible for converting enviroment and boat data from a text 
stream into accessible data members. Use state_rep.h to configure the input
and output formats.

Navigate
========

This module is the name for the high level decision maker. After main parses 
the file handler, this module reads from the file pointer parsing it into 
usable data for decision making.

Test
====

This module is used to unit test specific functions. Add a test main, with 
test data and the case to the compile script for anything you want to test.
