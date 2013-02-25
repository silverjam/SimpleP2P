// simplep2p.hxx

#ifndef simplep2p_hxx
#define simplep2p_hxx

class simplep2p_private;

//! Simple entrypoint for the plugin
class simplep2p
{
public:
    simplep2p();
    ~simplep2p();

    //! Send out a peer discovery packet
    void discover_peers();

    //! Retreive the number of peers that responded
    int get_peer_count();

    //! Get the name of the peer at an index
    const char* get_peer(int index);

    //! Send a message to a peer
    void send_message_to_peers(const char* message);

    //! The number of peers that responsed
    int response_count();

    //! Get the name of the peer that responded at an index
    const char* get_peer_of_response(int index);

    //! Get the text of the peer that responded at an index
    const char* get_text_of_response(int index);

private:
    simplep2p_private* d;
};

#endif // simplep2p_hxx
