							****************************
							**	StringHasher	  **
							****************************
							

The project is a server which will accept connections on a specific port(configurable), 
accept a string('\n' terminated) and then return a hash(SHA or STL hash calculation).

							************************
							*	Building       *
							************************
src/ directory contains all the code files and tests/ directory contains all the tests.
To build the code simply run `./build.sh`, this will create a folder `out` and place the
binary in that folder.
This script will check and install dependencies as well if needed.

							***********************
							*	Running	      *
							***********************
To run the solution you can do `./out/HashServer <port> <num_threads> <hash_type (SHA/STL)>`
The input parameters are to be given in order. For example 
`./out/HashServer <num_threads> <hash_type (SHA/STL)>` will assume that second paramter 
is a port and program will not run correctly.

You can skip the parameters in the ending order. For example 
`./out/HashServer <port> <num_threads>` is valid input and program will use SHA hashing by default.

Default Values of parameters if skipped are as follows;
<port> -> 1234
<num_threads> -> no. of cores
<hash_type> -> SHA

							**********************
							*	Client	     *
							**********************
I have tested the server with a telnet client. Example is given below;

`telnet 127.0.0.1 1234`

Once connected to server you can send any string and server weill respond back with hash 
of the string and then close the connection.

							*********************
							*	Tests	    *
							*********************
Tests are placed in `tests/` directory. In order to build the tests we can run `./tests/build_tests.sh`.

Unit tests are written using gtest suite. This script will check and install gtest 
dependencies as well if needed. This script will also create another script `/tests/run_tests.sh`. 
You can run this script to run all the tests.
