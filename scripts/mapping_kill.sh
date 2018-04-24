#!/bin/bash

echo "mapping kill bash."
kill -2 $(ps -au | grep "roslaunch micvision_mapping mapping_neo.launch"|grep "/usr/bin/python"|awk '{print $2}')
