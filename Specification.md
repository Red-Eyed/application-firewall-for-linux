<pre>

Software requirements specification<br>
<br>
<br>
Introduction<br>
Purpose<br>
Firewall for management process in linux.<br>
Project scope<br>
Study new techology, work in command, gaining experience working driver.<br>
<br>
Overall Description<br>
Product features<br>
Exist blacklist for pocesses.<br>
Exist process list.<br>
Add process to blacklist.<br>
Remove process from blacklist.<br>
Add not running processes to blacklist by path.<br>
Kill process.<br>
Display information about the selected process in the process list.<br>
<br>
User documentation<br>
Test plan.<br>
Code documentation.<br>
UI documentation.<br>
Diagram which show the work of application.<br>
Presentation.<br>
<br>
System features<br>
File AFFL_black_list : File which contains the list of processes should be<br>
added to the blacklist.<br>
File AFFL_command: File for communication between UI and driver.<br>
Add to blacklist: Adds the selected process to blacklist.<br>
Remove from blacklist: Removes the selected user from the user program, the<br>
process of the file that should be stored blacklist.<br>
Add not running processes blacklisted by name: Adds the path of the<br>
executable is not running, the file in the file for the blacklist.<br>
Kill process:  Kill the selected process.<br>
Display information about the selected process in the process list: Show information<br>
about the selected process.<br>
<br>
External interface requirements<br>
User interfaces<br>
The program uses the UI. There are two forms.<br>
In the first form:<br>
Field which displays a list of all processes.<br>
Field that displays the list of blocked processes (blacklist).<br>
Field that displays information about the selected process.<br>
<br>
5 buttons:<br>
Button that updates the list of processes.<br>
Button that adds the selected process in the blacklist.<br>
Button that removes the selected process from the blacklist.<br>
Button that kill the selected process.<br>
Button which open the second form.<br>
The second form:<br>
Input field to enter the path to the executable file.<br>
Button to add a name of executable file  to the blacklist.<br>
<br>
Communication interfaces<br>
Communication between the user program and the driver using the file<br>
(Describe the option / command) (see System features)<br>
<br>
Limitations<br>
The program must be run as root.<br>
Enter the path to the executable file, but not the name of the executable file.<br>
OS Linux (kubuntu 13.10, ubuntu 13.10, lubuntu 13.10). Kernel version 3.11.<br>
</pre>