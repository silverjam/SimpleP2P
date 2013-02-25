#!/usr/bin/env python

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
