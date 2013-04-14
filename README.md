# SimpleP2P

The following is a simple "P2P" libary, really, messages are multicast via UDP
to any client that's listening.  Clients tag their messages with a UUID so that
they don't act on their own message (since each client is both listening and
sending on the same multicast port).

## Instructions for building

I wrote this on Ubuntu 12.10, I installed the following packages:

    sudo apt-get install build-essential # probably already installed
    sudo apt-get install cmake
    sudo apt-get install libboost-dev
    sudo apt-get install libboost-system-dev
    sudo apt-get install libpython3.3-dev
    sudo apt-get install libboost-date-time1.49.0
    sudo apt-get install libboost-dev
    sudo apt-get install libboost-iostreams1.49.0
    sudo apt-get install libboost-system-dev
    sudo apt-get install libboost-system1.49-dev
    sudo apt-get install libboost-system1.49.0
    sudo apt-get install libboost1.49-dev

To build:

    cmake . && make

To test:

    ./runtest.sh

## Challenges encountered

This took more than four hours -- my wife is mad that I'm not doing chores :-P

I've written native Python extension modules before, so this wasn't much of a
challenge-- what was a little difficult was finding a suitable, simple example
that could be easily adapted fit my needs.

I decided not to built an actual object, and I only implemented the bare
minimum of what the pseudo code asked for (which was to gather all nodes
available, send a message, then verify that all nodes responded).

### Handling CTRL-C in a native Python extension module

This a little tricky-- not as straightforward as just adding a signal handler,
but my google-kung-foo found a suitable answer.

Handling this in boost::asio also took a little googling-- because I wanted to
cleanly tear down the asio server.

### Asynchronous vs synchronous

This was implemented using async methods from boost::asio (examples online were
easy to adapt)-- I used async to start because of considerations for how
something like this might be used in a larger application.  You could throw the
synchronous communications in a thread, but usually it's better to avoid the
threading and place the library in the event loop of the application.  To do
this though, you need to write things asynchronously and have a method that
allows the user to hook the library into the event loop.  For boost::asio, this
would be the io\_service::run\_one method.

### Figuring what exactly constitutes "P2P"

I started out implementing things in TCP, but decided that it'd take too much
time to figure out how to coordinate the clients.  Using multicast UDP seemed
like the easiest way to get something that resembled P2P.

## Limitations

This type of "P2P" is multicast-- it does not use a central server, DHT, or a
supernode to perform any sort of coordination between the peers.  Large amounts
of traffic will probably flood the network.

A better implementation would probably just use multicast for node discover,
then TCP, or plain UDP (non-multicast) for message transmission.

As for the multicast aspect, multicast usually doesn't travel past the local
network segment, so this code will only work with devices behind one router.

## The client must be written to be portable

### Platform, Libraries

The source uses boost, which is more than a multi platform layer, but it
provides this too.  Any networking, interaction with time libraries, and
anything else that's outside the standard library (std::) should go through
boost.  Except where standard C libraries can be used safely (in this library
::signal() was used).

### Build system

The build system used is CMake-- which provides cross-platform compilation.

## Simple test program in native code

The simple test program written in native code is contained in the
simplep2p-cli program, to run:

    cmake . && make
    ./src/simplep2p-cli recv &
    bg0=$!
    ./src/simplep2p-cli recv &
    bg1=$!
    ./src/simplep2p-cli recv &
    bg1=$!
    ./src/simplep2p-cli recv &
    bg1=$!
    ./src/simplep2p-cli recv &
    bg1=$!
    sleep 1
    ./src/simplep2p-cli send
    kill $bg0
    kill $bg1

## Create bindings in a managed language

The binding are written for Python, the module exposes several simple function:

    Help on module simplep2p:

    NAME
        simplep2p - SimpleP2P python module

    FILE
        /home/jam/Documents/GitHub/SimpleP2P/test/simplep2p.so

    FUNCTIONS
        get_peer_count(...)
            get a count of available peers
        
        send_message_to_peers(...)
            send a message to all available peers
        
        wait(...)
            wait for a message from a peer


The python module can be built with the _pybuildext.sh_ script.

## Create a test program in the managed language

The following test program is implemented in Python, the following script is in
_test/send.py_:

    import sys
    import simplep2p as s

    peer_count = s.get_peer_count()

    if peer_count == 0:
        print "FAIL: no peers"
        sys.exit(1)

    response_count = s.send_message_to_peers("hello_there")

    if response_count == peer_count:
        print "PASS"
    else:
        print "FAIL"
        sys.exit(1)

Starting a peer is even simpler, from _test/wait.py_:

    import simplep2p as s
    s.wait()

The Python test sequence can be lauched via the script: _test/runtest.sh_.

