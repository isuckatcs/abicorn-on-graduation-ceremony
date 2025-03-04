// RUN: abicorn --no-color --old old.cpp -p-old=. --new new.cpp -p-new=. -- 2>&1 | filecheck new.cpp
