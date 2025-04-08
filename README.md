# Miniprosjekt-AIS2105
# Miniprosjekt-AIS2105
## Kjøring av kuba med qube_ws
For å finne ut enhets navnet på kuba må man kjøre
```
ls /dev/tty*
```
Dette returnerer en liste der det riktige navnet vil være /dev/ttyACMX, der X er et tall. For at den skal kunne styres fra PCen må den få tilgang. 666 betyr at den får rettighet til å lese og skrive.
```
sudo chmod 666 /dev/ttyACMX
```
For at qube_driver skal fungere må man ha ROS2 control installert. Dette gjøres ved å kjøre
```
sudo apt install -y ros-jazzy-ros2-control ros-jazzy-ros2-controllers
```
## Kjøring av bringup
Kjør koden under i terminalen inne i workspacen.
```
colcon build
```
Dette bygger alle ROS2 pakkene. Etter at alle pakkene er bygd må man source også launche bringupen. Dette starter også driveren og rviz med beskrivelsen
```
source install/setup.bash
```
```
ros2 launch qubeBringup bringup.launch.py
```
## Kjøring av qubeController
I en annen terminal, men i samme lokasjon som der bringupen ble kjørt må man på nytt kjøre colcon build og source, også kjøres
```
ros2 run qubeController pid_node 
```
Dette starter PID kontrolleren og PID noda. PID kontrolleren har et problem med at den bare stiller seg inn til rett vinkel den ene veien. Hvorfor det er sånn veit jeg ikke.
For å sette en ny ønska vinkel, for eksempel 3.0 kjører man
```
ros2 param set /pid_node wanted_angle 3.0
```
## En demonstrasjon av at programmet virker


https://github.com/user-attachments/assets/14b7c226-b658-42b1-8172-1e14da20ffd5

