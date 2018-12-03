SET PATH=%PATH%;%1
echo %1
gcc -g main.c Graph.h GraphMinPath.h GraphIncidenceList.c generics/List.h generics/LinkedList.c -o %2
