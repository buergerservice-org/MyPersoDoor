gcc -I json -c jsoncpp.cpp
gcc -c workflowLibrary.cpp
ar rvs workflowLibrary.a jsoncpp.o workflowLibrary.o
