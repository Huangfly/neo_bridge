# neo_bridge

# Dependencies

1. [navigation](https://github.com/tyuownu/navigation)
2. [cartographer](https://github.com/tyuownu/cartographer)
3. [cartographer_ros](https://github.com/tyuownu/cartographer_ros)
4. [cartographer_turtlebot](https://github.com/tyuownu/cartographer_turtlebot)
5. [ceres-solver](https://github.com/tyuownu/ceres-solver)
6. [stage_ros](https://github.com/tyuownu/stage_ros)
7. [micvision](https://github.com/tyuownu/micvision)

# Install

Install simulation environment.
```shell
cd scripts/
bash install.sh neo
```

Install for turtlebot2
```shell
cd scripts/
bash install.sh turtlebot2
```

Install for turtlebot3
```shell
cd scripts/
bash install.sh turtlebot3
```

# usage

```shell
# robot
roslaunch neo_bridge StartNeo.launch
```

```shell
# stage_ros simulation
roslaunch neo_bridge NeoSim.launch
```
