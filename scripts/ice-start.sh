#!/bin/sh

killall icebox

icebox --daemon --Ice.Config="$1config/icebox_icestorm.cfg"
