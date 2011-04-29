#!/bin/sh

killall icebox

icebox --Ice.Config="$1config/icebox_icestorm.cfg"
