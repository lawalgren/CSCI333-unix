#!/bin/bash

cd ~/Projects;
for folder in $(ls); do 
    cd ~/Projects/$folder;
    echo "Pulling $folder";
    output=$(git pull) 
    echo $output | egrep -i "(warning)|(error)";
    if [$(echo $output | grep -i "Already up to date") != ""]; then
        echo "Already up to date";
    fi;
done;
echo "All done!";
