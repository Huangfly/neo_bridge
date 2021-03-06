#!/bin/bash

function inputLineToFile {

  if [ $# -eq 2 ]
  then
    echo $1 >> $2
  else
    echo "no paramer"
  fi
}

map_file_name=mapping.sh
mapkill_file_name=mapping_kill.sh
navigation_file_name=navigation.sh
navigationkill_file_name=navigation_kill.sh
bash_dir=~/.neo/bash
log_dir=~/.neo/log
maps_dir=~/.neo/maps
lua_dir=../configuration_files

# create config.lua
rm -rf $lua_dir/config.lua
if [ $# -eq 1 ] && [ $1 == neo ]
then
  cp $lua_dir/config_neo.lua $lua_dir/config.lua
elif [ $# -eq 1 ] && [ $1 == turtlebot2 ]
then
  cp $lua_dir/config_turtlebot2.lua $lua_dir/config.lua
elif [ $# -eq 1 ] && [ $1 == turtlebot3 ]
then
  cp $lua_dir/config_turtlebot3.lua $lua_dir/config.lua
else
  cp $lua_dir/config_neo.lua $lua_dir/config.lua
fi


mkdir -p $bash_dir
mkdir -p $log_dir
mkdir -p $maps_dir

cd $bash_dir

# remove file 
sudo rm -rf 	$map_file_name\
	 	$mapkill_file_name \
		$navigation_file_name \
		$navigationkill_file_name

# create file
sudo touch      $map_file_name\
	 	$mapkill_file_name \
		$navigation_file_name \
		$navigationkill_file_name

sudo chmod 777  $map_file_name \
		$mapkill_file_name \
		$navigation_file_name \
		$navigationkill_file_name


# create mapping.sh
inputLineToFile "#!/bin/bash" $map_file_name
inputLineToFile "echo \"mapping bash.\"" $map_file_name

# create mapping_kill.sh
inputLineToFile "#!/bin/bash" $mapkill_file_name
inputLineToFile "echo \"mapping kill bash.\"" $mapkill_file_name
inputLineToFile "kill -2 \$(ps -ef | grep \"roslaunch micvision_sim mapping_\"|grep \"/usr/bin/python\"|awk '{print \$2}')" $mapkill_file_name

# create navigation.sh
inputLineToFile "#!/bin/bash" $navigation_file_name
inputLineToFile "echo \"navigation bash.\"" $navigation_file_name

# create navigation_kill.sh
inputLineToFile "#!/bin/bash" $navigationkill_file_name
inputLineToFile "echo \"navigation kill bash.\"" $navigationkill_file_name
inputLineToFile "kill -2 \$(ps -ef | grep \"roslaunch micvision_sim navigation_\"|grep \"/usr/bin/python\"|awk '{print \$2}')" $navigationkill_file_name


if [ $# -eq 1 ] && [ $1 == neo ]
then
  echo "----create sim neo"
  inputLineToFile "roslaunch micvision_sim mapping_neo.launch > $log_dir/mapping.log" $map_file_name
  inputLineToFile "roslaunch micvision_sim navigation_neo.launch map_filename:=$maps_dir/navigation.yaml > $log_dir/navigation.log" $navigation_file_name
elif [ $# -eq 1 ] && [ $1 == turtlebot2 ]
then
  echo "----create turtlebot2"
  inputLineToFile "roslaunch micvision_sim mapping_turtlebot2.launch > $log_dir/mapping.log" $map_file_name
  inputLineToFile "roslaunch micvision_sim navigation_turtlebot2.launch map_filename:=$maps_dir/navigation.yaml > $log_dir/navigation.log" $navigation_file_name
elif [ $# -eq 1 ] && [ $1 == turtlebot3 ]
then
  echo "----create turtlebot2"
  inputLineToFile "roslaunch micvision_sim mapping_turtlebot3.launch > $log_dir/mapping.log" $map_file_name
  inputLineToFile "roslaunch micvision_sim navigation_turtlebot3.launch map_filename:=$maps_dir/navigation.yaml > $log_dir/navigation.log" $navigation_file_name
else
  echo "----remove all bash file"
  sudo rm -rf 	$map_file_name\
	 	$mapkill_file_name \
		$navigation_file_name \
		$navigationkill_file_name
fi

echo "install end."











