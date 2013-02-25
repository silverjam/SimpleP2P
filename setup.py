from distutils.core import setup, Extension

simplep2p = Extension(
    "simplep2p", 
    libraries = ["simplep2p", "boost_system-mt"],
    library_dirs = ["src"],
    sources = ["src/py_simplep2p.cpp"]
    )

setup(name="simplep2p", version="0.0", ext_modules = [simplep2p])
