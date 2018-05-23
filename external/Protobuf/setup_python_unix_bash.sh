#!/usr/bin/env bash

PATH=$PATH;$CD\bin
cd .\python-not-compiled
python setup.py build
python setup.py install