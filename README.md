# PersoDoor
With this program you can open a door with your german Personalausweis  

Version: 0.1 
Author: buergerservice.org e.V. <KeePerso@buergerservice.org>  


![Screenshot](screenshot1.jpg)

-------------
requirements:
-------------
Raspberry Pi with Raspberry Pi OS Bullseye (atm only in testmode avaiable but will be released soon)
program workflowClient for the admin   
AusweisApp2
cardreader or smartphone with AusweisApp2 on the same WLAN (connect in AusweisApp2-GUI) 
for online identification ready Personalausweis - you can test it in AusweisApp2 with "Meine Daten einsehen"  
internetaccess  


-------------
installation:
-------------


-----------
how to use:
-----------
start AusweisApp2
start persodoor from shell ./persodoor 
it logs to persodoor.log in the same directory


----------------------------
known problems and questions
----------------------------

is my PIN safe?  
	- the PIN is only sent to AusweisApp2 and not stored. you can use a cardreader with keypad or smartphone with activated PIN-input, then the plugin cant see the PIN.  

what data of my Personalausweis is used for the key?  
	like you can see in the source used are this data  
	FamilyNames (or BirthName if set)  
	GivenNames  
	DateOfBirth  
	PlaceOfBirth  

is an attackscenario possible where someone takes the source and builds a new Credential Provider where the data are not read  
from Personalausweis and the attacker writes them direct in the code and generates the key?  
	yes theoretical, but first he also must know the adminpassword and second if the admin keeps his pc safe noone can install  
	a attackerplugin or copy the database  



---------------
versionhistory:
---------------
0.1 start  


-----
build
-----
use the build-scripts ./build.sh




