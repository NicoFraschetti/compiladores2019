#!/bin/bash

dot -T pdf dot_output.dot -o dot_output.pdf
evince dot_output.pdf