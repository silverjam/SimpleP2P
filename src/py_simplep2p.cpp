#include "Python.h"

#include "simplep2p.hxx"

static simplep2p g_simplep2p;

PyDoc_STRVAR(simplep2p__doc__, "SimpleP2P python module");

PyDoc_STRVAR(get_peer_count__doc__, "get a count of available peers");

static PyObject *
py_get_peer_count(PyObject *self, PyObject *args)
{
    g_simplep2p.discover_peers();
	
	return PyInt_FromLong((long) g_simplep2p.get_peer_count());
}

PyDoc_STRVAR(send_message_to_peers__doc__, "send a message to all available peers");

static PyObject *
py_send_message_to_peers(PyObject *self, PyObject *args)
{
    char* message;
    int ok = PyArg_ParseTuple(args, "s", &message);

    if ( ! ok )
    {
        PyErr_SetString(PyExc_RuntimeError, "specify a message to send");
        return 0;
    }

    g_simplep2p.send_message_to_peers(message);
	
	return PyInt_FromLong((long) g_simplep2p.response_count());
}

volatile sig_atomic_t kb_interrupt = 0;

static void handle_sigint(int signal)
{
    kb_interrupt = 1;
    g_simplep2p.stop();
}

PyDoc_STRVAR(wait__doc__, "wait for a message from a peer");

static PyObject *
py_wait(PyObject *self, PyObject *args)
{
    __sighandler_t prev = signal(SIGINT, handle_sigint);
    g_simplep2p.wait();
    signal(SIGINT, prev);

    if ( kb_interrupt )
    {
        PyErr_SetObject(PyExc_KeyboardInterrupt, NULL);
        return NULL;
    }

    return Py_None;
}

static PyMethodDef simplep2p_methods[] = {
	{"get_peer_count",  py_get_peer_count, METH_VARARGS, get_peer_count__doc__},
	{"send_message_to_peers",  py_send_message_to_peers, METH_VARARGS, send_message_to_peers__doc__},
	{"wait", py_wait, METH_VARARGS, wait__doc__},
	{NULL, NULL}      /* sentinel */
};

PyMODINIT_FUNC
initsimplep2p(void)
{
	Py_InitModule3("simplep2p", simplep2p_methods, simplep2p__doc__);
}
