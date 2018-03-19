#!/bin/bash

if [ ! -d www.awkwardlysocialband.com ] ; then
    wget -r --level=2 http://www.awkwardlysocialband.com
fi 

find www.awkwardlysocialband.com -type f -a \
    \( -iname '*.html' -o -iname '*.htm' \) \
    -exec sed -i- -e 's;Awkwardly Social;MyBand;g' {} \;
