#!/bin/bash

echo "navigation kill bash."
kill -2 $(ps -au | grep "roslaunch micvision_navigation navigation_neo.launch"|grep "/usr/bin/python"|awk '{print $2}')
