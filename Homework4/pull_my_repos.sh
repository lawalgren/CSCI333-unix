#!/bin/bash

projectsDir=/home/lucas/projects

cd $projectsDir;
for folder in $(ls); do 
    cd $projectsDir/$folder;
    echo "Pulling $folder";
    output=$(git pull 2>&1); 
    errors=$(echo $output | egrep "warning|error");
    if [[ ! $errors == "" ]] ; then 
        echo "$errors";
        exit 1; 
    elif [[ ! $(echo $output | grep -i "Already up-to-date") == "" ]]; then
        echo "Already up to date";
    else
        echo "Pulled successfully";
    fi;
done;
echo "All done!";
exit 0;
