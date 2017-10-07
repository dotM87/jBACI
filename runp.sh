#
 if [ "$#" -eq  "0" ]
then
   java -jar ./jbaci.jar 
else
   java -jar ./jbaci.jar  $1.pm
fi
